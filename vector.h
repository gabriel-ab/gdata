/* 
 * Vector v1.0
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `typedef_vector(type)` and 
 *   use `typeVector` as your vector
 * 
 *   Ex:
 *      typedef_vector(int);
 *      intVector v = vector_create(int, {1,2,3});
 *      intVector v = vector_alloc(int, 10);
 *
 *   Obs:
 *     > You must call `vector_delete()` later
 */

#pragma once
#include <stddef.h>

/* 
 * ### Declare a type of vector
 */
# define typedef_vector(type)\
typedef struct type##Vector {\
    type* at;\
    size_t size;\
    struct {\
        size_t alloc;\
        size_t offset;\
        size_t dsize;\
        type* start;\
    } internal;\
} *type##Vector

# define vector_create(type, ...) ({\
    type arr[] = __VA_ARGS__;\
    (type##Vector)_vector_create(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})

# define vector_alloc(type, size)\
    (type##Vector)_vector_create(sizeof(type), size, 0)

/* 
 * ### get Vector Name
 */
#define Vector(type) type##Vector

// General functions

/* 
 * ### Create vector and pass values
 * ex:
 *    _vector_create(sizeof(int), 5, NULL)  -> [0,0,0,0,0]
 *    _vector_create(sizeof(int), 5, (int[]){1,2,3}) -> [1,2,3,0,0]
 */
void* _vector_create(size_t data_size, size_t size, void* values);

void vector_delete(void* v);

/*
 * ### Push `data` to vector's tail
 * ex:
 *     vec_push_back(vector, (int[]){ 12 });
 *     int value = 10;
 *     vec_push_back(vector, &value);
 */
void vec_push_back(void* vector, void* data);

// ### Pop vector's last element
void* vec_pop_back(void* vector);

/* ### Push `data` to vector's head
 * ex:
 *     vec_push_front(vector, (int[]){ 12 });
 *     int value = 10;
 *     vec_push_front(vector, &value);
 */
void vec_push_front(void* vector, void* data);

// ### Pop vector's first element
void* vec_pop_front(void* vector);

// Declaring basic data vectors
typedef_vector(int);
typedef_vector(char);
typedef_vector(long);
typedef_vector(short);
typedef_vector(float);
typedef_vector(double);