#include "gdata/array.h"
#include <string.h>

// Create a new Array with values, if values are passed.
AnyArray array_create(size_t dsize, size_t initial_size, void* initial_values) {
    size_t * array = malloc(sizeof(size_t) + dsize * initial_size);
    if (array) {
        *array = initial_size;
        if (initial_values) 
            memcpy(array + 1, initial_values, dsize * initial_size);
        else
            memset(array + 1, 0, initial_size * dsize);
    }
    return array;
}

void array_resize(AnyArray *array, const size_t dsize, const size_t new_size) {
    AnyArray ptr = *array;
    ptr = realloc(ptr, sizeof(size_t) + dsize*new_size);
    *(size_t*)ptr = new_size;
    *array = ptr;
}

AnyArray array_join(AnyArray a, AnyArray b, size_t dsize) {
    // array 1
    char* a_data = (char*)a + sizeof(size_t);
    size_t a_size = *(size_t*)a;

    // array 2
    char* b_data = (char*)b + sizeof(size_t);
    size_t b_size = *(size_t*)b;

    // result
    char* result = array_create(dsize, a_size + b_size, 0);
    char* data = result + sizeof(size_t);

    memcpy(data, a_data, a_size*dsize);
    data += a_size * dsize; // jump to the end
    memcpy(data, b_data, b_size*dsize);
    return result;
}

void array_append(AnyArray *a, AnyArray b, size_t dsize) {
    void* array = *a;
    size_t a_size = *(size_t*)a;
    size_t b_size = *(size_t*)b;
    array_resize(&array, dsize, a_size + b_size);

    char* a_data = array + sizeof(size_t);
    char* b_data = (char*)b + sizeof(size_t);
    char* joint = a_data + a_size*dsize;
    memcpy(joint, b_data, b_size*dsize);
    *a = array;
}