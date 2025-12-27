#ifndef ASYNC_H
#define ASYNC_H

#include <stdatomic.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

// Opaque forward declaration (users don't need to know internals)
typedef struct Future Future;

// Public macros
#define async void *
#define await(fut) future_await(fut)
#define ASYNC_SPAWN(fn, arg) async_spawn((async_fn)(fn), (arg))

// Public type for async functions
typedef async (*async_fn)(void *arg);

// Public functions
void executor_init(int num_threads);
void executor_deinit(void);

Future *async_spawn(async_fn fn, void *arg);
void *future_await(Future *fut);
void future_destroy(Future *fut);

// Optional: for advanced use (e.g., manual completion)
// But usually not needed — tasks complete themselves
void future_complete(Future *f, void *result);

#endif // ASYNC_H
