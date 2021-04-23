/**
 * Vector v1.1
 * 
 * @author: Gabriel-AB
 * @github: https://github.com/Gabriel-AB/
 * 
 * usage:
 *      call `VECTOR_TYPEDEF(type)` and 
 *      use `typeVector` as your vector
 * 
 * ex:
 *      VECTOR_TYPEDEF(int);
 *      intVector v = VECTOR_CREATE(int, 1,2,3);
 *      intVector v = VECTOR_ALLOCATE(int, 10);
 * 
 * build in types:
 *      Vector       intVector     floatVector 
 *      charVector   shortVector   doubleVector
 *                   longVector 
 */
#pragma once
#include <stddef.h>

/**
 * @brief Declare a type of vector and use `typeVector`
 * @usage:
 *      VECTOR_TYPEDEF(MyType);
 *      and MyTypeVector is now available
 * 
 * @note: type must be a single name, because the type must fit in vector's name
 *        so, combined words types like: `struct myStruct`, `enum myEnum` 
 *        or `unsigned int` does not work.
 * 
 *        To use any of them, define a new type using `typedef`
 */
#define VECTOR_TYPEDEF(type)\
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

// Declaring basic data vectors
VECTOR_TYPEDEF(char);

VECTOR_TYPEDEF(short);
VECTOR_TYPEDEF(int);
VECTOR_TYPEDEF(long);

VECTOR_TYPEDEF(float);
VECTOR_TYPEDEF(double);

typedef void* AnyVector;


// ===== MACROS ===== //


/**
 * @brief Creates a new vector with values if passed
 * @note: You must call `vector_delete()` later
 * 
 * @param type: any defined type. ex: int, float, etc...
 * @param __VA_ARGS__: values to initialize vector
 */
#define VECTOR_CREATE(type, ...) ({\
    __typeof__(type) _vec[] = {__VA_ARGS__};\
    vector_create(sizeof(type), sizeof(_vec)/sizeof(*_vec), _vec);\
})

/**
 * @brief Creates a new vector with given size.
 * All elements are set to zero
 * 
 * @note: You must call `vector_delete()` later
 */
#define VECTOR_ALLOCATE(type, size)\
    vector_create(sizeof(type), size, 0)

/**
 * @brief Push one or more values to vector's end
 * @param vector: any type of vector
 * @param __VA_ARGS__: values to push, the type inside list or literal
 */
#define VECTOR_PUSHBACK(vector, ...) ({\
    __typeof__(*vector->at) _vec[] = {__VA_ARGS__};\
    vector_pushback(vector, sizeof(_vec)/sizeof(*_vec), _vec);\
})

/**
 * @brief Push one or more values to vector's end
 * @param vector: any type of vector
 * @param __VA_ARGS__: values to push, the type inside list or literal
 */
#define VECTOR_PUSHFRONT(vector, ...) ({\
    __typeof__(*vector->at) _vec[] = {__VA_ARGS__};\
    vector_pushfront(vector, sizeof(_vec)/sizeof(*_vec), _vec);\
})

/**
 * @brief Get the last element and remove it from vector
 * @return value
 */
#define VECTOR_POPBACK(vector) (*(__typeof__(vector->at))vector_popback(vector))

/**
 * @brief Get the first element and remove it from vector
 * @return value
 */
#define VECTOR_POPFRONT(vector) (*(__typeof__(vector->at))vector_popfront(vector))


// ===== FUNCTIONS ===== //

/**
 * @brief Create vector and pass values
 * 
 * @param dsize: size of each element in bytes
 * @param initial_size: initial size of the list. (0 is valid)
 * @param initial_values: pointer to data that will be pushed first. (0 is valid)
 */
AnyVector vector_create(size_t dsize, size_t initial_size, void* initial_values);

/**
 * @brief Push data to vector's end.
 * 
 * @param vector: Vector, intVector, floatVector, etc...
 * @param num_elements: number of elements in data
 * @param data: array with values to be pushed (values will be copied)
 * 
 * @see VECTOR_PUSHBACK() macro
 */
void vector_pushback(AnyVector vector, size_t num_elements, void* data);

/**
 * @brief Push data to vector's begin.
 * 
 * @param vector: Vector, intVector, floatVector, etc...
 * @param num_elements: number of elements in data
 * @param data: array with values to be pushed (values will be copied)
 * 
 * @see VECTOR_PUSHFRONT()
 */
void vector_pushfront(AnyVector vector, size_t num_elements, void* data);

/**
 * @brief Pop vector's last element
 * 
 * @param vector: Vector, intVector, floatVector, etc...
 * 
 * @returns: reference to value
 * 
 * @see VECTOR_POPBACK(), it returns value instead of reference
 */
void* vector_popback(AnyVector vector);

/**
 * @brief Pop vector's first element
 * 
 * @param vector: Vector, intVector, floatVector, etc...
 * 
 * @returns: reference to value
 * 
 * @see VECTOR_POPFRONT(), it returns value instead of reference
 */
void* vector_popfront(AnyVector vector);

// Remove element at given index
void vector_remove(AnyVector vector, size_t index);

// Get a pointer to the given index of a vector.
void* vector_at(void* vector, size_t index);

// Destructor
void vector_delete(AnyVector vector);
