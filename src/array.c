#include "array.h"
#include <string.h>

// Create a new Array with values, if values are passed.
void* array_create(size_t dsize, size_t initial_size, void* initial_values) {
    charArray array = malloc(sizeof(*array) + dsize * initial_size);
    if (array) {
        *(size_t*)&array->size = initial_size;
        *(size_t*)&array->internal.dsize = dsize;
        if (initial_values)
            memcpy(array->at, initial_values, dsize * initial_size);
        else
            memset(array->at, 0, initial_size * dsize);
    }
    return array;
}

void* array_resize(void* array, const size_t new_size) {
    charArray A = array;
    array = realloc(array, sizeof(*A) + A->internal.dsize*new_size);
    *(size_t*)&A->size = new_size;
    return array;
}

void* array_join(void* a, void* b) {
    charArray A = a, B = b;
    charArray result = array_create(A->internal.dsize, A->size + B->size, 0);

    size_t bytes_size = A->size*A->internal.dsize;
    memcpy(result->at, A->at, bytes_size);
    
    bytes_size = B->size*B->internal.dsize;
    memcpy(result->at + A->size*A->internal.dsize, B->at, bytes_size);
    return result;
}

void* array_slice(void* array, unsigned int begin, unsigned int end) {
    charArray A = array;
    size_t size = end - begin;
    void* initial_values = A->at + begin*A->internal.dsize;
    return array_create(A->internal.dsize, size, initial_values);
}

bool array_equals(void* a, void* b) {
    charArray A = a, B = b;
    if (A->size != B->size || A->internal.dsize != B->internal.dsize)
        return false;
    size_t dsize = A->internal.dsize;
    return memcmp(A->at, B->at, dsize*A->size) == 0;
}