#include "Array.h"
#include <memory.h>

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //


void _array_realloc(Array array, size_t new_size) {
    array->size = new_size;
    array = realloc(array, sizeof(struct array) + new_size*array->data_size);
}

// Returns the address of data for the given index
void * _array_at(Array array, unsigned int index) {
    return array->data + array->data_size*index;
}

 
// Create a new Array with values, if values are passed.
// if value == NULL or 0, calloc already sets every value to 0
Array _array_create(size_t data_size, size_t size, void * values) {
    Array array = calloc(1,sizeof(struct array) + size*data_size);
    array->data_size = data_size;
    array->size = size;
    array->data = array + sizeof(struct array);

    if (values)
        for (int i = 0; i < array->size; i++) 
            memcpy(_array_at(array, i), values + array->data_size*i, array->data_size);
    return array;
}

void Array_resize(Array array, size_t new_size) {
    if (new_size > array->size || new_size < array->size * 0.2)
        _array_realloc(array, new_size);
    else
        array->size = new_size;
}

// Return one data per time in the array, then resets
void * Array_forEach(Array array) {
    if (array->iterator < array->size)
        return _array_at(array, array->iterator++);
    array->iterator = 0;
    return NULL;
}

// Set all values to 0
void Array_clear(Array array) {
    if (array->valueDestructor)
        for (void *data; data = Array_forEach(array);)
            array->valueDestructor(data);
    memset(array->data, 0, array->size * array->data_size);
}

/* 
 * ### Destructor of Array
 * Args:
 * * `array` is a reference
 * obs: `valueDestructor` must take data by reference.
 */
void ArrayDelete(Array *array) {
    Array_clear(*array);
    free(*array);
}