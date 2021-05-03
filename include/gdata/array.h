/* 
 * Heap Allocated Array v1.1
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `ARRAY_TYPEDEF(type)` before all the code 
 *   and use `typeArray` as your a array of type
 * 
 *   Ex: 
 *      ARRAY_TYPEDEF(int);
 *      intArray array1 = ARRAY_CREATE(int, {1,2,3});
 *      intArray array2 = ARRAY_ALLOCATE(int, 10);
 *
 *   Obs:
 *     > You must call `free(array)` later
 */
#pragma once
#include <stdlib.h>

/**
 * @brief Declares a array of type.
 * The type `typeArray` is will be avaliable to you
 */
#define ARRAY_TYPEDEF(type)\
typedef struct {\
    size_t size;\
    type at[];\
} *type##Array

// Declaring basic data arrays
ARRAY_TYPEDEF(char);

ARRAY_TYPEDEF(short);
ARRAY_TYPEDEF(int);
ARRAY_TYPEDEF(long);

ARRAY_TYPEDEF(float);
ARRAY_TYPEDEF(double);

typedef void* AnyArray;


// ===== MACROS ===== //


/**
 * @brief Create a new Array with zeros.
 * 
 * Obs: you must call `free(array)` later
 */
#define ARRAY_ALLOCATE(type, size) (type##Array)array_create(sizeof(type), size, 0)


/**
 * @brief Create a new Array based on a set of values.
 * 
 * @param type: any defined type. ex: int, float, etc...
 * @param __VA_ARGS__: values in curly braces. ex: {1,2,3}
 *
 * usage:
 *      ARRAY_CREATE(int, {0,2,4})
 *      ARRAY_CREATE(float, {2.3f, 0.1f})
 * 
 * Obs: you must call `free(array)` later
 */
#define ARRAY_CREATE(type, ...) ({\
    type _arr[] = __VA_ARGS__;\
    (type##Array)array_create(sizeof(type), sizeof(_arr)/sizeof(type), _arr);\
})

/// @brief Resize a array
#define ARRAY_RESIZE(array, new_size)\
    array_resize(&array, sizeof(*array->at), new_size)

/// Create a new array combining `a` and `b`
#define ARRAY_JOIN(a,b) array_join(a,b,sizeof(*a->at))

/// Appends `b` to `a`
#define ARRAY_APPEND(a,b) array_append(&a, b,sizeof(*a->at))


// ===== FUNCTIONS ===== //


/**
 * @brief Create a new Array with values
 * 
 * @param dsize: size of each element in bytes
 * @param size: initial size of the list. (non zero value)
 * @param initial_values: array of data to be pushed first. (NULL set all elements to 0)
 */
AnyArray array_create(size_t dsize, size_t size, void *initial_values);

/** 
 * @brief Reallocates an existing array (see: ARRAY_RESIZE macro)
 * 
 * @param array: reference to array
 * @param dsize: size of each element in bytes
 */
void array_resize(AnyArray *array, size_t dsize, size_t new_size);

/// Create a new array combining `a` and `b` (see: ARRAY_JOIN macro)
AnyArray array_join(AnyArray a, AnyArray b, size_t dsize);

/// appends `b` to `a` (see: ARRAY_APPEND macro)
void array_append(AnyArray *a, AnyArray b, size_t dsize);