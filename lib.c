#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int init_arr(DynamicArray *arr, size_t initCapacity) {
  if (initCapacity <= 0) {
    return ARRAY_ERR_EMPTY;
  }
  arr->data = (int *)malloc(initCapacity * sizeof(int));
  if (arr->data == NULL) {
    return ARRAY_ERR_MEMORY_ALLOC;
  }
  arr->size = 0;
  arr->capacity = initCapacity;
  printf("Array initialized with capacity %zu\n", initCapacity);
  return ARRAY_OK;
}

int push_back(DynamicArray *arr, int element) {
  if (arr->size == arr->capacity) {
    size_t newCapacity = arr->capacity * 2;
    printf("Resizing array: %zu -> %zu\n", arr->capacity, newCapacity);

    int *newData = (int *)realloc(arr->data, newCapacity * sizeof(int));
    if (newData == NULL) {
      return ARRAY_ERR_MEMORY_REALLOC;
    }

    arr->data = newData;
    arr->capacity = newCapacity;
  }

  arr->data[arr->size++] = element;
  return ARRAY_OK;
}

void free_array(DynamicArray *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
  printf("Memory freed.\n");
}

int remove_element(DynamicArray *arr, size_t index) {
  if (index < 0 || index >= arr->size) {
    return ARRAY_ERR_INDEX;
  }
  for (size_t i = index; i < arr->size - 1; i++) {
    arr->data[i] = arr->data[i + 1];
  }
  arr->size--;
  size_t newCapacity = arr->size;
  int *tmp = realloc(arr->data, newCapacity * sizeof(int));
  if (tmp != NULL) {
    arr->data = tmp;
    arr->capacity = newCapacity;
  } else {
    return ARRAY_ERR_MEMORY_REALLOC;
  }
  return ARRAY_OK;
}
