/* 
 * Generic Heap Allocated Array Library v0.1
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `USING_ARRAY_OF(type)` and use `typeArray`
 * 
 *   Ex: 
 *      USING_ARRAY_OF(int);
 *      intArray array = ArrayCreate(int, {1,2,3});
 *
 *   Obs:
 *     > You must call `free(array)` later
 * 
 *     > if needed, use sizeof(struct array) since `typeArray` is a pointer
 */
#pragma once
#include <stdlib.h>

/* 
 * ### Declares a array of type
 * The type `typeArray` is will be avaliable to you
 */
#define USING_ARRAY_OF(type)\
typedef struct {\
    size_t size;\
    type * at;\
} *type##Array

// Generic Array
struct array {
    size_t size;
    void * at;
};

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
/* 
 * ### Create a new Array with values
 */
struct array * _array_alloc(size_t data_size, size_t size, void * values);

/* 
 * ### Reallocates an existing array
 */
struct array * _array_realloc(struct array *array, size_t data_size, size_t new_size);


// =========================== PUBLIC FUNCTIONS =========================== //

/*
 * ### Create a new Array with zeros
 * 
 * Note: Call `USING_ARRAY_OF(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayAllocate(type, size)\
    (type##Array)_array_alloc(sizeof(type), size, 0)

/* 
 * ### Create a new Array based on a array literal
 * 
 * Ex: ArrayCreate(int, {0,2,4})
 * Ex: ArrayCreate(float, {})
 * 
 * Note: Call `USING_ARRAY_OF(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayCreate(type, array...) ({\
    type arr[] = array;\
    (type##Array)_array_alloc(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})


/* 
 * ### Resize a array and return the new array
 * return: the new array
 */
#define ArrayResize(old_array, new_size)\
    (__typeof__(old_array))_array_realloc((struct array*)old_array, sizeof(*(old_array->at))), new_size)
