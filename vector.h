/**
 * Vector v1.1
 * 
 * @author: Gabriel-AB
 * @github: https://github.com/Gabriel-AB/
 * 
 * usage:
 *      call `vector_typedef(type)` and 
 *      use `typeVector` as your vector
 * 
 * ex:
 *      vector_typedef(int);
 *      intVector v = vector_create(int, 1,2,3);
 *      intVector v = vector_allocate(int, 10);
 * 
 * build in types:
 *      Vector       intVector     floatVector 
 *      charVector   shortVector   doubleVector
 *                   longVector 
 */
#pragma once
#include <stddef.h>

/**
 * ### Declare a type of vector
 * and use `typeVector`
 * usage: 
 *      vector_typedef(MyType);
 *      and MyTypeVector is now available
 * 
 * note: type must be a single name, because the type must fit in vector's name
 *       so, combined words types like: `struct myStruct`, `enum myEnum` 
 *       or `unsigned int` does not work.
 * 
 *       To use any of them, define a new type using `typedef`
 *       
 *      
 */
#define vector_typedef(type)\
typedef struct type ## Vector {\
    type *at;\
    size_t size;\
    struct {\
        type *begin;\
        size_t offset;\
        size_t alloc;\
        size_t dsize;\
    } internal;\
}* type ## Vector





/**
 * ## Creates a new vector with values if passed
 * note: You must call `vector_delete()` later
 * 
 * _usage_:
 * ### common types:
 * > `vector_create(int)`
 * results in: empty vector
 *   
 * > `vector_create(int, 1, 3, 5)`
 * results in: [1, 2, 3]
 *   
 * > `vector_create(char, "string")`
 * results in: [`s`,`t`,`r`,`i`,`n`,`g`]
 * 
 *   
 * ### using arrays:
 * first define your type of array, ex: int3 for int[3]
 * ```
 * > typedef int int3[3] 
 * > vector_typedef(int3)
 * > int3Vector v1 = vector_create(int[3], 1,2,3, 0,0,0)
 * // OR
 * > int3Vector v2 = vector_create(int[3], {1,2,3}, {0,0,0})
 * ```
 * results in: [(1,2,3), (0,0,0)]
 * 
 *     
 * ### using strings: 
 * ```
 * > typedef char char32[32];
 * > vector_typedef(char32);
 * > char32Vector v = vector_create(char[32], "string 1", "string 2");
 * ```
 * results in: ["string 1", "string 2"]
 * 
 */
#define vector_create(type, ...) ({\
    __typeof__(type) v[] = {__VA_ARGS__};\
    _vec_create(sizeof(type), sizeof(v)/sizeof(*v), v);\
})

/**
 * Creates a new vector with given size
 * all elements are set to zero
 * 
 * note: You must call `vector_delete()` later
 */
#define vector_allocate(type, size)\
    _vec_create(sizeof(type), size, 0)

/**
 * Push one or more values to vector's end
 * usage:
 * ```
 * > vector_pushback(int_vector, 0)
 * > vector_pushback(int_vector, 1, 2, 3)
 * 
 * > vector_pushback(char_vector, "appended")
 * > vector_pushback(char_vector, 'a', 'd', 'd')
 * 
 * > vector_pushback(float_vector, 3.1, 9.21)
 * 
 * > vector_pushback(char32_vector, "string 1", "string 2")
 * ```
 * see `vector_create` for `char32` explanation
 */
#define vector_pushback(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    _vec_pushback(vector, sizeof(v)/sizeof(*v), v);\
})

/**
 * Push one or more values to vector's end
 * usage:
 * ```
 * > vector_pushfront(int_vector, 0)
 * > vector_pushfront(int_vector, 1, 2, 3)
 * 
 * > vector_pushfront(char_vector, "appended")
 * > vector_pushfront(char_vector, 'a', 'd', 'd')
 * 
 * > vector_pushfront(float_vector, 3.1, 9.21)
 * 
 * > vector_pushfront(char32_vector, "string 1", "string 2")
 * ```
 * see `vector_create` for `char32` explanation
 * note: multiple values will be pushed in the same order
 */
#define vector_pushfront(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    _vec_pushfront(vector, sizeof(v)/sizeof(*v), v);\
})

/**
 * Gets the last element
 * and remove it from vector
 */
#define vector_popback(vector) (*(__typeof__(vector->at))_vec_popback(vector));

/**
 * Gets the first element
 * and remove it from vector
 */
#define vector_popfront(vector) (*(__typeof__(vector->at))_vec_popfront(vector));


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

// Create vector and pass values
void* _vec_create(size_t data_size, size_t initial_size, void* initial_values);

// Push `data` to vector's end
void _vec_pushback(void* vector, size_t num_elements, void* data);

// Push `data` to vector's begin
void _vec_pushfront(void* vector, size_t num_elements, void* data);

// Pop vector's last element
void* _vec_popback(void* vector);

// Pop vector's first element
void* _vec_popfront(void* vector);

/**
 * ### Remove element at given index
 */
void vector_remove(void* vector, size_t index);

/**
 * ### Get a pointer to the given index
 * needed when using as generic Vector
 */
void* vector_at(void* vector, size_t index);

/**
 * ### Free it's internal array and structure 
 */
void vector_delete(void* vector);


// Declaring basic data vectors
vector_typedef(char);

vector_typedef(short);
vector_typedef(int);
vector_typedef(long);

vector_typedef(float);
vector_typedef(double);
