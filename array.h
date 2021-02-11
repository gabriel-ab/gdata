/* 
 * Heap Allocated Array v1.0
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `typedef_array(type)` before all the code 
 *   and use `typeArray` as your a array of type
 * 
 *   Ex: 
 *      typedef_array(int);
 *      intArray array1 = array_create(int, {1,2,3});
 *      intArray array2 = array_allocate(int, 10);
 *
 *   Obs:
 *     > You must call `free(array)` later
 */
#pragma once
#include <stddef.h>

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
// ### Create a new Array with values
void * _array_alloc(size_t data_size, size_t size, void * values);

// ### Reallocates an existing array
void _array_realloc(void *array, size_t data_size, size_t new_size);

// ### Create a new array combining `a` and `b`
void* _array_join(void* a, void* b, size_t data_size);

// ### appends `b` to `a`
void _array_append(void* a, void* b, size_t data_size);

// ============================= PUBLIC MACROS ============================ //

/* 
 * ### Declares a array of type
 * The type `typeArray` is will be avaliable to you
 * 
 * if you want to use a type like `unsigned int`, make an alias
 * ex: 
 * > typedef unsigned int uint; 
 * > typedef_array(uint);
 */
#define typedef_array(type)\
typedef struct {\
    size_t size;\
    type at[];\
} *type##Array


/* ### Create a new Array with zeros
 * 
 * Obs: you must call `free(array)` later
 */
#define array_allocate(type, size)\
    (type##Array)_array_alloc(sizeof(type), size, 0)


/* ### Create a new Array based on a set of values
 * 
 * equivalent to:
 *      type array[] = {values...};
 * 
 * usage:
 *      array_create(int, {0,2,4})
 *      array_create(float, {2.3f, 0.1f})
 * 
 * Obs: you must call `free(array)` later
 */
#define array_create(type, ...) ({\
    type arr[] = __VA_ARGS__;\
    (type##Array)_array_alloc(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})


/* ### Resize a array and return it
 * the array pointer can change, so, reassign it
 */
#define array_resize(old_array, new_size)\
    _array_realloc(&old_array, sizeof(*old_array->at), new_size)


// ### Create a new array combining `a` and `b`
#define array_join(a,b) _array_join(a,b,sizeof(*a->at))


// ### Appends `b` to `a`
#define array_append(a,b) _array_append(&a, b,sizeof(*a->at))


// Declaring basic data arrays
typedef_array(int);
typedef_array(char);
typedef_array(long);
typedef_array(short);
typedef_array(float);
typedef_array(double);