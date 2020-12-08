/* 
 * Generic Heap Allocated Array Library v0.1
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 */

#pragma once
#include <stdlib.h>

typedef struct array {
    void * data;
    size_t size;
    size_t iterator;
    size_t data_size;
    void (*valueDestructor)(void *value);
} *Array;


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
/* 
 * ### Create a new Array with values
 * Args:
 * * data_size: size of data in bytes
 * * size: quantity of elements in the array
 * * values: a vector with the values to insert in the array,
 *   pass `0` or `NULL` to set all values to 0
 */
Array _array_create(size_t data_size, size_t size, void * values);


// =========================== PUBLIC FUNCTIONS =========================== //
/* 
 * ### Resizes and reallocates memory to the array
 */
void Array_resize(Array array, size_t new_size);

/* 
 * ### return one data per time in the array, then resets.
 * You must expect `NULL` as the end
 * 
 * Usage: 
 * - `for (type *data; data = Array_forEach(list);) {}`
 */
void * Array_forEach(Array array);

/* 
 * ### Set all elements in the array to 0
 * it tries to use valueDestructor if it was setted
 */
void Array_clear(Array array);
/* 
 * To get or set values
 * 
 * Usage: `Array_at(type, array, index)`
 * 
 * Ex: `Array_at(double, array, 2) = 0.2;`
 * Ex: `int var = Array_at(int, array, 1);`
 */
#define Array_at(type, array, index) ((type*)array->data)[index]

/*
 * ### Create a new empty Array
 * Args:
 * * type: int, char, YourType...
 * * size: number of elements
 */
#define ArrayCreateEmpty(type, size) _array_create(sizeof(type), size, 0)

/* 
 * ### Create a new Array
 * Args:
 * * type: int, char, YourType...
 * * array: a array literal {}
 * Ex: ArrayCreate(int, {0,2,4})
 */
#define ArrayCreate(type, array...) ({\
    type arr[] = array;\
    _array_create(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})

/* 
 * ### Destructor of Array
 * Args:
 * * `array` is a pointer to your array
 */
void ArrayDelete(Array *array);