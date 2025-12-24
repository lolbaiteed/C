#include "lib/array.h"
#include "utils/errorHelper.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  DynamicArray arr;
  if (check_on_error(array_initialize(&arr)) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }
  fprintf(stdout, "Created array with capacity: 2\n");

  for (int i = 10; i <= 40; i += 10) {
    if (check_on_error(array_push(&arr, i)) != EXIT_SUCCESS) {
      array_free(&arr);
      return EXIT_FAILURE;
    }
  }
  fprintf(stdout, "Capacity after push: %zu\n", arr.capacity);

  fprintf(stdout, "Array elements: \n");
  for (size_t i = 0; i < arr.size; i++) {
    fprintf(stdout, "%d\n", arr.data[i]);
  }
  fprintf(stdout, "\n");

  if (check_on_error(array_remove_by_index(&arr, 0)) != 0) {
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Array elements after remove: \n");
  for (size_t i = 0; i < arr.size; i++) {
    fprintf(stdout, "%d \n", arr.data[i]);
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "Capacity after remove: %zu", arr.capacity);

  array_free(&arr);
  return EXIT_SUCCESS;
}
