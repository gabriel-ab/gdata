#include "array.h"
#include <string.h>

// Create a new Array with values, if values are passed.
void* array_create(size_t dsize, size_t initial_size, void* initial_values) {
    charArray array = malloc(sizeof(*array) + dsize * initial_size);
    if (array) {
        *(size_t*)&array->length = initial_size;
        *(size_t*)&array->dsize = dsize;
        if (initial_values)
            memcpy(array->at, initial_values, dsize * initial_size);
        else
            memset(array->at, 0, initial_size * dsize);
    }
    return array;
}

void* array_resize(void* array, const size_t new_size) {
    charArray A = array;
    array = realloc(array, sizeof(*A) + A->dsize*new_size);
    *(size_t*)&A->length = new_size;
    return array;
}

void* array_join(void* a, void* b) {
    charArray A = a, B = b;
    charArray result = array_create(A->dsize, A->length + B->length, 0);

    size_t bytes_size = A->length*A->dsize;
    memcpy(result->at, A->at, bytes_size);
    
    bytes_size = B->length*B->dsize;
    memcpy(result->at + A->length*A->dsize, B->at, bytes_size);
    return result;
}

void* array_slice(void* array, unsigned int begin, unsigned int end) {
    charArray A = array;
    size_t size = end - begin;
    void* initial_values = A->at + begin*A->dsize;
    return array_create(A->dsize, size, initial_values);
}

bool array_equals(void* a, void* b) {
    charArray A = a, B = b;
    if (A->length != B->length || A->dsize != B->dsize)
        return false;
    size_t dsize = A->dsize;
    return memcmp(A->at, B->at, dsize*A->length) == 0;
}