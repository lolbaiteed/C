#include <stdatomic.h>
#define _GNU_SOURCE
#include "async.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _WIN32
typedef HANDLE thread_t;
typedef CRITICAL_SECTION mutex_t;
typedef CONDITION_VARIABLE cond_t;
#define THREAD_RETURN DWORD WINAPI
#define thread_create(thread, attr, start, arg)                                \
  (*(thread) = CreateThread(NULL, 0, (start), (arg), 0, NULL)) != NULL
#define thread_join(thread, result) WaitForSingleObject((thread), INFINITE)
#define mutex_init(m) InitializeCriticalSection(m)
#define mutex_lock(m) EnterCriticalSection(m)
#define mutex_unlock(m) LeaveCriticalSection(m)
#define mutex_destroy(m) DeleteCriticalSection(m)
#define cond_init(c) InitializeConditionVariable(c)
#define cond_wait(c, m) SleepConditionVariableCS((c), (m), INFINITE)
#define cond_signal(c) WakeConditionVariable(c)
#define cond_broadcast(c) WakeAllConditionVariable(c)
#define cond_destroy(c) // No destroy for CONDITION_VARIABLE
#else                   // POSIX
typedef pthread_t thread_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;
#define THREAD_RETURN void *
#define thread_create(thread, attr, start, arg)                                \
  pthread_create((thread), (attr), (start), (arg)) == 0
#define thread_join(thread, result) pthread_join((thread), (result)) == 0
#define mutex_init(m) pthread_mutex_init((m), NULL)
#define mutex_lock(m) pthread_mutex_lock(m)
#define mutex_unlock(m) pthread_mutex_unlock(m)
#define mutex_destroy(m) pthread_mutex_destroy(m)
#define cond_init(c) pthread_cond_init((c), NULL)
#define cond_wait(c, m) pthread_cond_wait((c), (m))
#define cond_signal(c) pthread_cond_signal(c)
#define cond_broadcast(c) pthread_cond_broadcast(c)
#define cond_destroy(c) pthread_cond_destroy(c)
#endif

// -------------------- Future --------------------
struct Future {
  void *result;
  _Atomic(bool) done;
  mutex_t mutex;
  cond_t cond;
};

static Future *future_new(void) {
  Future *f = malloc(sizeof(Future));
  if (!f)
    return NULL;
  f->result = NULL;
  atomic_init(&f->done, false);
  mutex_init(&f->mutex);
  cond_init(&f->cond);
  return f;
}

void *future_await(Future *f) {
  mutex_lock(&f->mutex);
  while (!atomic_load(&f->done)) {
    cond_wait(&f->cond, &f->mutex);
  }
  mutex_unlock(&f->mutex);
  return f->result;
}

void future_complete(Future *f, void *result) {
  mutex_lock(&f->mutex);
  f->result = result;
  atomic_store(&f->done, true);
  cond_broadcast(&f->cond);
  mutex_unlock(&f->mutex);
}

// -------------------- Task & Queue --------------------

typedef struct {
  async_fn fn;
  void *arg;
  Future *future;
} Task;

typedef struct {
  Task *tasks;
  int capacity;
  int head, tail, count;
  mutex_t mutex;
  cond_t not_empty;
  cond_t not_full;
  _Atomic(bool) deinit;
} TaskQueue;

static TaskQueue queue;
static thread_t *workers = NULL;
static int num_workers = 0;

// init queue
static void queue_init(int capacity) {
  queue.tasks = malloc(capacity * sizeof(Task));
  queue.capacity = capacity;
  queue.head = queue.tail = queue.count = 0;
  mutex_init(&queue.mutex);
  cond_init(&queue.not_empty);
  cond_init(&queue.not_full);
  atomic_init(&queue.deinit, false);
}

static void queue_push(Task task) {
  mutex_lock(&queue.mutex);
  while (queue.count == queue.capacity && !atomic_load(&queue.deinit)) {
    cond_wait(&queue.not_full, &queue.mutex);
  }
  if (atomic_load(&queue.deinit)) {
    mutex_unlock(&queue.mutex);
    return;
  }
  // place at tail, then advance tail
  queue.tasks[queue.tail] = task;
  queue.tail = (queue.tail + 1) % queue.capacity;
  queue.count++;
  cond_signal(&queue.not_empty);
  mutex_unlock(&queue.mutex);
}

static bool queue_pop(Task *task) {
  mutex_lock(&queue.mutex);
  while (queue.count == 0 && !atomic_load(&queue.deinit)) {
    cond_wait(&queue.not_empty, &queue.mutex);
  }
  if (queue.count == 0) {
    // either deinit or spurious wake up with empty queue
    mutex_unlock(&queue.mutex);
    return false;
  }
  // take from head, then advance head
  *task = queue.tasks[queue.head];
  queue.head = (queue.head + 1) % queue.capacity;
  queue.count--;
  cond_signal(&queue.not_full);
  mutex_unlock(&queue.mutex);
  return true;
}

// -------------------- Worker --------------------
static THREAD_RETURN worker_thread(void *arg) {
  (void)arg;
  Task task;
  while (queue_pop(&task)) {
    // run task
    void *result = task.fn(task.arg);
    if (task.future) {
      future_complete(task.future, result);
    }
  }
  return 0;
}

// -------------------- Public API --------------------
void executor_init(int n_threads) {
  if (n_threads <= 0) {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    n_threads = sysinfo.dwNumberOfProcessors;
#else
    n_threads = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    if (n_threads <= 0)
      n_threads = 4;
  }
  num_workers = n_threads;
  workers = malloc(n_threads * sizeof(thread_t));
  if (!workers) {
    fprintf(stderr, "failed to allocate workers\n");
    return;
  }
  queue_init(128);

  for (int i = 0; i < n_threads; i++) {
    thread_create(&workers[i], NULL, worker_thread, NULL);
  }
}

void executor_deinit(void) {
  atomic_store(&queue.deinit, true);

  mutex_lock(&queue.mutex);
  cond_broadcast(&queue.not_empty);
  cond_broadcast(&queue.not_full);
  mutex_unlock(&queue.mutex);

  for (int i = 0; i < num_workers; i++) {
    thread_join(workers[i], NULL);
  }

  free(workers);
  workers = NULL;

  if (queue.tasks) {
    free(queue.tasks);
    queue.tasks = NULL;
  }
  // Note: not destroying mutex/conds for simplicity, but you can add
  mutex_destroy(&queue.mutex);
  cond_destroy(&queue.not_empty);
  cond_destroy(&queue.not_full);
}

Future *async_spawn(async_fn fn, void *arg) {
  Future *fut = future_new();
  if (!fut)
    return NULL;

  Task task = {.fn = fn, .arg = arg, .future = fut};
  queue_push(task);
  return fut;
}
