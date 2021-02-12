/* 
 * Vector v1.1
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 * Usage:
 *   call `vector_typedef(type)` and 
 *   use `typeVector` as your vector
 * 
 *   Ex:
 *      vector_typedef(int);
 *      intVector v = vector_create(int, 1,2,3);
 *      intVector v = vector_allocate(int, 10);
 *
 *   Obs:
 *      You must call `vector_delete()` later
 */
#pragma once
#include <stddef.h>

/* 
 * ### Declare a type of vector
 * and use `typeVector`

 * usage: 
 *      vector_typedef(MyType);
 *      and MyTypeVector is now available
 * 
 * obs: this does NOT work with combined word types like:
 *      struct myStruct, enum myEnum, unsigned int
 *      
 */
#define vector_typedef(type)\
typedef struct type##Vector {\
    type* at;\
    size_t size;\
    struct {\
        type* begin;\
        size_t offset;\
        size_t alloc;\
        size_t dsize;\
    } internal;\
} *type##Vector

/* 
 * ### Creates a new vector with values if passed
 * usage:
 *      vector_create(int)
 *      vector_create(int, 1,3,5)
 * 
 * obs: You must call `vector_delete()` later
 */
#define vector_create(type, ...) ({\
    type v[] = {__VA_ARGS__};\
    (type##Vector)_vec_create(sizeof(type), sizeof(v)/sizeof(type), v);\
})

/* 
 * ### Creates a new vector with given size
 * all elements are set to zero
 * 
 * obs: You must call `vector_delete()` later
 */
#define vector_allocate(type, size)\
    (type##Vector)_vec_create(sizeof(type), size, 0)

/* 
 * ### Push one or more values to vector's end
 * usage:
 *      vector_pushback(int_vector, 0)
 *      vector_pushback(int_vector, 0, 1, 2)
 */
#define vector_pushback(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    _vec_pushback(vector, sizeof(v)/sizeof(*v), v);\
})

/* 
 * ### Push one or more values to vector's end
 * usage:
 *      vector_pushfront(int_vector, 0)
 *      vector_pushfront(int_vector, 0, 1, 2) 
 * 
 * obs: multiple values will be pushed in the same order
 */
#define vector_pushfront(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    _vec_pushfront(vector, sizeof(v)/sizeof(*v), v);\
})

/* 
 * ### Gets the last element
 * and remove it from vector
 */
#define vector_popback(vector) (*(__typeof__(vector->at))_vec_popback(vector));

/* 
 * ### Gets the first element
 * and remove it from vector
 */
#define vector_popfront(vector) (*(__typeof__(vector->at))_vec_popfront(vector));

/* 
 * ### get Vector Name
 */
#define vector(type) type##Vector

// General functions

/* 
 * ### Create vector and pass values
 * ex:
 *    _vec_create(sizeof(int), 5, NULL)  -> [0,0,0,0,0]
 *    _vec_create(sizeof(int), 5, (int[]){1,2,3}) -> [1,2,3,0,0]
 */
void* _vec_create(size_t data_size, size_t size, void* values);

// free it's internal array and it self
void vector_delete(void* v);

// Push `n` elements of `data` to vector's tail
void _vec_pushback(void* vector, size_t n, void* data);
// Push `n` elements of `data` to vector's head
void _vec_pushfront(void* vector, size_t n, void* data);

// Pop vector's last element
void* _vec_popback(void* vector);
// Pop vector's first element
void* _vec_popfront(void* vector);

// Declaring basic data vectors
vector_typedef(int);
vector_typedef(char);
vector_typedef(long);
vector_typedef(short);
vector_typedef(float);
vector_typedef(double);