#include "array.h"
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

void _array_realloc(void* array, size_t data_size, size_t new_size) {
    void* ptr = *(void**)array;
    ptr = realloc(ptr, sizeof(size_t) + data_size*new_size);
    *(size_t*)ptr = new_size;
    *(void**)array = ptr;
}

void* _array_join(void* a, void* b, size_t type_size) {

    size_t a_size = *(size_t*)a;
    size_t b_size = *(size_t*)b;

    void* a_data = a + sizeof(size_t);
    void* b_data = b + sizeof(size_t);

    void* result = _array_alloc(type_size, a_size + b_size, 0);


    void* data = result + sizeof(size_t); // points to the first part
    memcpy(data, a_data, a_size*type_size);

    data += a_size * type_size; // points to the second part
    memcpy(data, b_data, b_size*type_size);
    return result;
}

// a is passed by reference
void _array_append(void* a, void* b, size_t data_size) {
    void* _a = *(void**)a;
    size_t a_size = *(size_t*)_a;
    size_t b_size = *(size_t*)b;
    _array_realloc(&_a, data_size, a_size + b_size);

    void* a_data = _a + sizeof(size_t);
    void* b_data =  b + sizeof(size_t);
    void* joint = a_data + a_size*data_size;
    memcpy(joint, b_data, b_size*data_size);
    *(void**)a = _a;
}