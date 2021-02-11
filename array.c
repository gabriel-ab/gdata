#include "array.h"
#include <stdlib.h>
#include <string.h>

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

// Create a new Array with values, if values are passed.
void * _array_alloc(size_t data_size, size_t size, void * values) {
    size_t * array = malloc(sizeof(size_t) + data_size*size);
    if (array) {
        *array = size;
        if (values) 
            memcpy(array + 1, values, data_size*size);
        else
            memset(array + 1, 0, size*data_size);
    }
    return (void*)array;
}

void _array_realloc(void* array, const size_t data_size, const size_t new_size) {
    void* ptr = *(void**)array;
    ptr = realloc(ptr, sizeof(size_t) + data_size*new_size);
    *(size_t*)ptr = new_size;
    *(void**)array = ptr;
}

void* _array_join(void* a, void* b, size_t data_size) {
    // array 1
    char* a_data = (char*)a + sizeof(size_t);
    size_t a_size = *(size_t*)a;

    // array 2
    char* b_data = (char*)b + sizeof(size_t);
    size_t b_size = *(size_t*)b;

    // result
    char* result = _array_alloc(data_size, a_size + b_size, 0);
    char* data = result + sizeof(size_t);

    memcpy(data, a_data, a_size*data_size);
    data += a_size * data_size; // jump to the end
    memcpy(data, b_data, b_size*data_size);
    return result;
}

// a is passed by reference
void _array_append(void* a, void* b, size_t data_size) {
    char* array = *(void**)a;
    size_t a_size = *(size_t*)a;
    size_t b_size = *(size_t*)b;
    _array_realloc(&array, data_size, a_size + b_size);

    char* a_data = array + sizeof(size_t);
    char* b_data = (char*)b + sizeof(size_t);
    char* joint = a_data + a_size*data_size;
    memcpy(joint, b_data, b_size*data_size);
    *(void**)a = array;
}