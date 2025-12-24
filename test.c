#include "src/lib/async.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

async compute_fib(void* arg) {
  long n = (intptr_t)arg;
  if (n <= 1) return (void*)n;
  long a = 0, b = 1;
  for (long i = 2; i <= n; i++) {
    long next = a + b;
    a = b;
    b = next;
    for (volatile int j = 0; j < 10000000; j++);
  }
  return (void*)b;
}

async parallel_fib_sum(void* arg) {
  (void)arg;

  printf("Spawning fib(40) and fib(38) in parallel...\n");

  Future* f1 = ASYNC_SPAWN(compute_fib, (void*)(intptr_t)40);
  Future* f2 = ASYNC_SPAWN(compute_fib, (void*)(intptr_t)38);

  long r1 = (intptr_t)await(f1);
  long r2 = (intptr_t)await(f2);

  printf("fib(40) = %ld\n", r1);
  printf("fib(38) = %ld\n", r2);

  return (void*)(r1 + r2);
}

int main(int argc, char *argv[])
{
  printf("Staring async runtime...\n");
  executor_init(8);

  Future* main_task = async_spawn(parallel_fib_sum, NULL);
  long total = (intptr_t)await(main_task);

  printf("Total sum: %ld\n", total);

  executor_deinit();

  printf("done\n");
  return EXIT_SUCCESS;
}

