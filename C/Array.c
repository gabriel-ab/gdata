#include "Array.h"
#include <string.h>

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

// Create a new Array with values, if values are passed.
// if value == NULL or 0, calloc already sets every value to 0
struct array * _array_alloc(size_t data_size, size_t size, void * values) {
    struct array *array = calloc(1,sizeof(struct array) + data_size*size);
    array->size = size;
    array->at = array + sizeof(struct array);

    if (values)
        memcpy(array->at, values, data_size*size);
    return array;
}

struct array * _array_realloc(struct array *array, size_t data_size, size_t new_size) {
    array = realloc(array, sizeof(struct array)+data_size*new_size);
    array->size = new_size;
    return array;
}