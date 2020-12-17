#include "array.h"
#include "autofree.h"
#include <string.h>



// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

// Create a new Array with values, if values are passed.
// if value == NULL or 0, calloc already sets every value to 0
void * _array_alloc(size_t data_size, size_t size, void * values) {
    size_t * array = calloc(1, sizeof(size_t) + data_size*size);
    *array = size;
    if (values)
        memcpy(array + 1, values, data_size*size);
    return (void*)array;
}

void * _array_realloc(void *array, size_t data_size, size_t new_size) {
    array = realloc(array, sizeof(size_t) + data_size*new_size);
    *(size_t*)array = new_size;
    return array;
}