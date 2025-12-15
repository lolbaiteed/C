#include "lib.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  DynamicArray arr;
//FIX: need wait for user input
  fprintf(stdout,"Insert a initial capacity of array: "); 
  size_t initCapacity;
  fscanf(stdin, "%ld", &initCapacity); 

  if (check_on_error(init_arr(&arr, initCapacity)) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  } else {
    printf("Array initialized with capacity %zu\n", initCapacity);
  }

  for (int i = 10; i <= 40; i += 10) {
    if (check_on_error(push_back(&arr, i)) != EXIT_SUCCESS) {
      free_array(&arr);
      return EXIT_FAILURE;
    }
  }

  printf("Array elements: \n");
  for (size_t i = 0; i < arr.size; i++) {
    printf("%d\n", arr.data[i]);
  }

  if (check_on_error(remove_element(&arr, 0)) != 0) {
    return EXIT_FAILURE;
  }

  printf("Array elements after remove: \n");
  for (size_t i = 0; i < arr.size; i++) {
    printf("%d \n", arr.data[i]);
  }

  free_array(&arr);
  return EXIT_SUCCESS;
}
