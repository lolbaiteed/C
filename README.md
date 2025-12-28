# C Async/Await Library

A lightweight, header-only(ish) async/await runtime for C, inspired by modern JavaScript/Rust syntax. It provides a thread-pool-based executor and a macro-based DSL for defining and awaiting asynchronous tasks with minimal boilerplate.

## Features

*   **Simple Syntax**: Use `ASYNC`, `await`, and `return promise(val)` to write asynchronous code.
*   **Automatic Resource Management**: `await(promise)` consumes and frees the promise.
*   **Flexible Returns**: Supports `long`, `int`, `char*`, `void`, etc.

## Installation

Include the following files:
*   `src/lib/async.h`
*   `src/lib/async.c`

Link with `-pthread` on POSIX.

## API Usage

### 1. Define Async Functions
Use the `ASYNC` macro. Return values with `promise()`.

```c
// Returns a long
ASYNC(long, add, void*, arg) {
    long n = (long)(intptr_t)arg;
    return promise(n + 10);
}

// Returns a string
ASYNC(char*, greet, void*, arg) {
    return promise("Hello");
}
```

### 2. Await Results
Use `await()` for all types.

*   **Numbers (`long`, `int`)**:
    No cast needed. `await` returns `intptr_t` by default, which assigns cleanly to integers.
    ```c
    Promise *p = add((void*)5);
    long res = await(p); 
    ```

*   **Pointers (`char*`, `structs`)**:
    Use a standard C cast.
    ```c
    Promise *p = greet(NULL);
    char *str = (char*)await(p); 
    ```

*   **Void (Ignore result)**:
    ```c
    Promise *p = do_something(NULL);
    await(p); 
    ```

## Example
```c
int main() {
    executor_init(4);

    Promise *p1 = add((void*)10);
    Promise *p2 = greet(NULL);

    long num = await(p1);
    char *msg = (char*)await(p2);

    printf("%s: %ld\n", msg, num);

    executor_deinit();
    return 0;
}
```
