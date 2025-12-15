#include "utils.h"
#include "lib.h"
#include <stdio.h>

int check_on_error(int returnVal) {
  switch (returnVal) {
  case ARRAY_ERR_EMPTY:
    fprintf(stderr, "ERROR: Cannot initialize empty array\n");
    return EXIT_FAILURE;
  case ARRAY_ERR_MEMORY_ALLOC:
    fprintf(stderr, "ERROR: Cannot allocate memory\n");
    return EXIT_FAILURE;
  case ARRAY_ERR_MEMORY_REALLOC:
    fprintf(stderr, "ERROR: Cannot reallocate memory\n");
    return EXIT_FAILURE;
  case ARRAY_ERR_INDEX:
    fprintf(stderr, "ERROR: Cannot remove element from index\n");
    return EXIT_FAILURE;
  default:
    return EXIT_SUCCESS;
  }
}
