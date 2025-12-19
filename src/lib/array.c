#include "array.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int array_initialize(DynamicArray *arr, size_t initCapacity) {
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;

  if (initCapacity < 0) {
    if (initCapacity > SIZE_MAX / sizeof(int)) {
      return ARRAY_ERR_EMPTY;
    }
    arr->data = (int *)malloc(initCapacity * sizeof(int));
    if (arr->data == NULL) {
      return ARRAY_ERR_MEMORY_ALLOC;
    }
    arr->capacity = initCapacity;
  }
  return ARRAY_OK;
}

int array_push(DynamicArray *arr, int element) {
  if (arr->size == arr->capacity) {
    size_t newCapacity = (arr->capacity == 0) ? 2 : arr->capacity * 2;

    if (newCapacity <= arr->capacity || newCapacity > SIZE_MAX / sizeof(int)) {
      return ARRAY_ERR_MEMORY_OVERFLOW;
    }

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

void array_free(DynamicArray *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
}

int array_remove_by_index(DynamicArray *arr, size_t index) {
  if (index >= arr->size) {
    return ARRAY_ERR_INDEX;
  }
  for (size_t i = index; i < arr->size - 1; i++) {
    arr->data[i] = arr->data[i + 1];
  }
  arr->size--;
  
  if (arr->size > 0 && arr->size * 4 <= arr->capacity) {
    size_t newCapacity = arr->capacity / 2;
    if (newCapacity < 2) newCapacity = 2;
    int *tmp = realloc(arr->data, newCapacity * sizeof(int));
    if (tmp != NULL) {
      arr->data = tmp;
      arr->capacity = newCapacity;
    } else {
      return ARRAY_ERR_MEMORY_REALLOC;
    }
  }
  return ARRAY_OK;
}
