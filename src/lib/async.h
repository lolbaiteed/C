#ifndef ASYNC_H
#define ASYNC_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

// Opaque forward declaration
typedef struct Future Future;

// Generic Promise structure
typedef struct Promise {
    Future *future;
} Promise;

// Public type for async functions (raw task)
typedef void * (*async_fn)(void *arg);

// Core Executor Functions
void executor_init(int num_threads);
void executor_deinit(void);
Future *async_spawn(async_fn fn, void *arg);
void *future_await(Future *fut);
void future_destroy(Future *fut);
void future_complete(Future *f, void *result);

// --- Promise System ---

// Spawns a task and returns a generic Promise
Promise *promise_create(Future *f);

// Awaits the promise, destroys it, and returns the result
void *promise_await_consume(Promise *p);

// Macro for async function definition
#define async(ReturnType, FuncName, ArgType, ArgName) \
    void* FuncName##_task(ArgType ArgName); \
    Promise *FuncName(ArgType ArgName) { \
        Future *f = async_spawn((async_fn)FuncName##_task, (void*)(intptr_t)ArgName); \
        return promise_create(f); \
    } \
    void* FuncName##_task(ArgType ArgName)

// Helper to wrap return value
#define promise(val) ((void*)(intptr_t)(val))

// Await macro: awaits and destroys. 
// Casts to intptr_t by default.
// This allows direct assignment to integers/longs.
// For pointers, use a standard C cast: char *s = (char*)await(p);
#define await(p) ((intptr_t)promise_await_consume(p))

#endif // ASYNC_H
