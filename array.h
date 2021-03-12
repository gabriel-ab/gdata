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
#include <stddef.h>

/* 
 * ### Declares a array of type
 * The type `typeArray` is will be avaliable to you
 * 
 * if you want to use a type like `unsigned int`, make an alias
 * ex: 
 * > typedef unsigned int uint; 
 * > ARRAY_TYPEDEF(uint);
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


/* ### Create a new Array with zeros
 * 
 * Obs: you must call `free(array)` later
 */
#define ARRAY_ALLOCATE(type, size)\
    (type##Array)array_create(sizeof(type), size, 0)


/* ### Create a new Array based on a set of values
 * 
 * equivalent to:
 *      type array[] = {values...};
 * 
 * usage:
 *      ARRAY_CREATE(int, {0,2,4})
 *      ARRAY_CREATE(float, {2.3f, 0.1f})
 * 
 * Obs: you must call `free(array)` later
 */
#define ARRAY_CREATE(type, ...) ({\
    type arr[] = __VA_ARGS__;\
    (type##Array)array_create(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})

/* ### Resize a array and return it
 * the array pointer can change, so, reassign it
 */
#define ARRAY_RESIZE(array, new_size)\
    array_resize(&array, sizeof(*array->at), new_size)

// ### Create a new array combining `a` and `b`
#define ARRAY_JOIN(a,b) array_join(a,b,sizeof(*a->at))

// ### Appends `b` to `a`
#define ARRAY_APPEND(a,b) array_append(&a, b,sizeof(*a->at))


// ===== FUNCTIONS ===== //


// ### Create a new Array with values
void* array_create(size_t data_size, size_t size, void *initial_values);

// ### Reallocates an existing array
void array_resize(AnyArray *array, size_t data_size, size_t new_size);

// ### Create a new array combining `a` and `b`
AnyArray array_join(AnyArray a, AnyArray b, size_t data_size);

// ### appends `b` to `a`
void array_append(AnyArray *a, AnyArray b, size_t data_size);