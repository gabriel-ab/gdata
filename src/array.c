#include "gdata/array.h"
#include <string.h>

// Create a new Array with values, if values are passed.
AnyArray array_create(size_t dsize, size_t initial_size, void* initial_values) {
    charArray *array = malloc(sizeof(*array) + dsize * initial_size);
    if (array) {
        array->size = initial_size;
        array->dsize = dsize;
        if (initial_values)
            memcpy(array->at, initial_values, dsize * initial_size);
        else
            memset(array->at, 0, initial_size * dsize);
    }
    return array;
}

AnyArray array_resize(AnyArray array, const size_t new_size) {
    charArray *arr = array;
    array = realloc(array, sizeof(*arr) + arr->dsize*new_size);
    arr->size = new_size;
    return array;
}

AnyArray array_join(AnyArray a, AnyArray b) {
    charArray *A = a, *B = b;
    charArray *result = array_create(A->dsize, A->size + B->size, 0);

    size_t bytes_size = A->size*A->dsize;
    memcpy(result->at, A->at, bytes_size);
    
    bytes_size = B->size*B->dsize;
    memcpy(result->at + A->size*A->dsize, B->at, bytes_size);
    return result;
}

AnyArray array_slice(AnyArray array, unsigned int begin, unsigned int end) {
    charArray *arr = array;
    size_t size = end - begin;
    void* initial_values = arr->at + begin*arr->dsize;
    return array_create(arr->dsize, size, initial_values);
}

bool array_equals(AnyArray a, AnyArray b) {
    charArray *A = a, *B = b;
    if (A->size != B->size || A->dsize != B->dsize)
        return false;
    return array_equals_data(a, B->at);
}

bool array_equals_data(AnyArray array, void* data) {
    charArray *arr = array;
    for (size_t i = 0; i < arr->size; i++) {
        size_t current_index = i*arr->dsize;
        void* current_a = arr->at + current_index;
        void* current_b = data + current_index;
        
        if (memcmp(current_a, current_b, arr->dsize) != 0)
            return false;
    }
    return true;
}