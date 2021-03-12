#include "array.h"
#include <stdlib.h>
#include <string.h>

// Create a new Array with values, if values are passed.
void * array_create(size_t data_size, size_t initial_size, void* initial_values) {
    size_t * array = malloc(sizeof(size_t) + data_size * initial_size);
    if (array) {
        *array = initial_size;
        if (initial_values) 
            memcpy(array + 1, initial_values, data_size * initial_size);
        else
            memset(array + 1, 0, initial_size * data_size);
    }
    return (void*)array;
}

void array_resize(AnyArray *array, const size_t data_size, const size_t new_size) {
    AnyArray ptr = *array;
    ptr = realloc(ptr, sizeof(size_t) + data_size*new_size);
    *(size_t*)ptr = new_size;
    *array = ptr;
}

AnyArray array_join(AnyArray a, AnyArray b, size_t data_size) {
    // array 1
    char* a_data = (char*)a + sizeof(size_t);
    size_t a_size = *(size_t*)a;

    // array 2
    char* b_data = (char*)b + sizeof(size_t);
    size_t b_size = *(size_t*)b;

    // result
    char* result = array_create(data_size, a_size + b_size, 0);
    char* data = result + sizeof(size_t);

    memcpy(data, a_data, a_size*data_size);
    data += a_size * data_size; // jump to the end
    memcpy(data, b_data, b_size*data_size);
    return result;
}

void array_append(AnyArray *a, AnyArray b, size_t data_size) {
    void* array = *a;
    size_t a_size = *(size_t*)a;
    size_t b_size = *(size_t*)b;
    array_resize(&array, data_size, a_size + b_size);

    char* a_data = array + sizeof(size_t);
    char* b_data = (char*)b + sizeof(size_t);
    char* joint = a_data + a_size*data_size;
    memcpy(joint, b_data, b_size*data_size);
    *a = array;
}