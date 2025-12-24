#ifndef LIB_H
#define LIB_H

#include <stdlib.h>

typedef struct {
  int *data;
  size_t size;
  size_t capacity;
} DynamicArray;

typedef enum {
  ARRAY_OK = 0,
  ARRAY_ERR_INDEX,
  ARRAY_ERR_MEMORY_ALLOC,
  ARRAY_ERR_MEMORY_REALLOC,
  ARRAY_ERR_MEMORY_OVERFLOW,
  ARRAY_ERR_EMPTY,
} ArrayError;

/**
 * @param DynamicArray *arr - Pointer to array which must be initialized
 * @param int initCapacity - Value of array initial capacity, cannot be 0
 * @return ARRAY_ERR_EMPTY if initCapacity is 0 or above ARRAY_ERR_MEMORY_ALLOC,
 * if no memory allocated to array, ARRAY_OK on success
 */
int array_initialize(DynamicArray *arr);

/**
 * @param DynamicArray *arr - Pointer to array
 * @param element - number which must be pushed to array
 * @return ARRAY_ERR_MEMORY_REALLOC if memory cannot be reallocated otherwise
 * ARRAY_OK
 */
int array_push(DynamicArray *arr, int element);

/**
 * @breif This function free array from memory
 * @param DynamicArray *arr - Pointer to array
 * @return void
 */
void array_free(DynamicArray *arr);

/**
 * @param DynamicArray *arr - Pointer to array
 * @param size_t index - index of array taht must be removed
 * @return ARRAY_ERR_INDEX if requested index is < 0 or > than array size,
 * ARRAY_ERR_MEMORY_REALLOC if memory cannot be reallocated, otherwise ARRAY_OK
 */
int array_remove_by_index(DynamicArray *arr, size_t index);

#endif // !LIB_H
