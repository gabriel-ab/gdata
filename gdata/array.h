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
 */
#pragma once
#include <stdlib.h>

/* 
 * ### Declares a array of type
 * The type `typeArray` is will be avaliable to you
 * 
 * if you want to use a type like `unsigned int`, make an alias
 * ex: 
 * > typedef unsigned int uint; 
 * > ArrayDeclare(uint);
 */
#define ArrayDeclare(type)\
typedef struct {\
    size_t size;\
    type at[];\
} *type##Array


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
// ### Create a new Array with values
void * _array_alloc(size_t data_size, size_t size, void * values);

// ### Reallocates an existing array
void _array_realloc(void *array, size_t data_size, size_t new_size);

// ### Create a new array combining `a` and `b`
void* _array_join(void* a, void* b, size_t data_size);

// ### appends `b` to `a`
void _array_append(void* a, void* b, size_t data_size);

// =========================== PUBLIC FUNCTIONS =========================== //

/* ### Create a new Array with zeros
 * 
 * Note: Call `ArrayDeclare(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayAllocate(type, size)\
    _array_alloc(sizeof(type), size, 0)


/* ### Create a new Array based on a array literal and assign values
 * 
 * equivalent to:
 *      type array[] = {};
 * 
 * usage:
 *      ArrayCreate(int, {0,2,4})
 *      ArrayCreate(float, {2.3f, 0.1f})
 * 
 * Note: Call `ArrayDeclare(type)` before using this macro
 * Obs: you must call `free(array)` later
 */
#define ArrayCreate(type, array...) ({\
    type arr[] = array;\
    _array_alloc(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})


/* ### Resize a array and return it
 * the array pointer can change, so, reassign it
 */
#define ArrayResize(old_array, new_size)\
    _array_realloc(&old_array, sizeof(*old_array->at), new_size)


// ### Create a new array combining `a` and `b`
#define ArrayJoin(a,b) _array_join(a,b,sizeof(*a->at))


// ### Appends `b` to `a`
#define ArrayAppend(a,b) _array_append(&a, b,sizeof(*a->at))


// Declaring basic data arrays
ArrayDeclare(int);
ArrayDeclare(char);
ArrayDeclare(long);
ArrayDeclare(short);
ArrayDeclare(float);
ArrayDeclare(double);