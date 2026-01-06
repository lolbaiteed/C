#include "src/lib/async.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Async function returning a long
async(long, compute_fib, void*, arg) {
  long n = (intptr_t)arg;
  if (n <= 1) return promise(n);
  
  long a = 0, b = 1;
  for (long i = 2; i <= n; i++) {
    long next = a + b;
    a = b;
    b = next;
    // busy wait
    for (volatile int j = 0; j < 10000000; j++);
  }
  
  return promise(b);
}

// Async function returning a string (char*)
async(char*, get_greeting, void*, arg) {
    (void)arg;
    return promise("Hello from Async World!"); 
}

// Async function returning void (essentially)
async(void, print_msg, void*, arg) {
    printf(">> Task: %s\n", (char*)arg);
    return promise(NULL);
}

int main(void) {
  printf("Staring async runtime...\n");
  executor_init(0);

  // 1. Long Return - No cast needed
  printf("\n--- Test 1: Long ---\n");
  Promise* fib_task = compute_fib((void*)(intptr_t)10);
  long fib_res = await(fib_task); 
  printf("fib(10) = %ld\n", fib_res);

  // 2. String Return - Standard C cast
  printf("\n--- Test 2: String ---\n");
  Promise* str_task = get_greeting(NULL);
  char* msg = (char*)await(str_task); // Standard cast
  printf("Greeting: %s\n", msg);

  // 3. Void/No Return
  printf("\n--- Test 3: Void ---\n");
  Promise* void_task = print_msg("Executing side effect...");
  await(void_task); 
  printf("Side effect finished.\n");

  executor_deinit();
  printf("\ndone\n");
  return EXIT_SUCCESS;
}
