/* 
 * Heap Allocated Array Library v1.0
 * Do not free, it will be freed automatically
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `ArrayDeclare(type)` before all the code 
 *   and use `typeArray` as your a array of type
 * 
 *   Ex: 
 *      ArrayDeclare(int);
 *      intArray array1 = ArrayCreate(int, {1,2,3});
 *      intArray array2 = ArrayAllocate(int, 10);
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
#define ArrayDeclare(type)\
typedef struct {\
    size_t size;\
    type at[];\
} *type##Array


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
/* 
 * ### Create a new Array with values
 */
void * _array_alloc(size_t data_size, size_t size, void * values);

/* 
 * ### Reallocates an existing array
 */
void * _array_realloc(void *array, size_t data_size, size_t new_size);


// =========================== PUBLIC FUNCTIONS =========================== //

/*
 * ### Create a new Array with zeros
 * 
 * Note: Call `ArrayDeclare(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayAllocate(type, size)\
    _array_alloc(sizeof(type), size, 0)

/* 
 * ### Create a new Array based on a array literal and assign values
 * 
 * Ex: ArrayCreate(int, {0,2,4})
 * Ex: ArrayCreate(float, {2.3f, 0.1f})
 * 
 * Note: Call `ArrayDeclare(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayCreate(type, array...) ({\
    type arr[] = array;\
    _array_alloc(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})


/* 
 * ### Resize a array and return the new array
 * return: the new array
 */
#define ArrayResize(old_array, new_size)\
    _array_realloc(old_array, sizeof(*old_array->at), new_size)
