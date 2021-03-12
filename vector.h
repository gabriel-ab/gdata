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
 * ### Declare a type of vector
 * and use `typeVector`
 * usage: 
 *      VECTOR_TYPEDEF(MyType);
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
 * ## Creates a new vector with values if passed
 * note: You must call `vector_delete()` later
 * 
 * _usage_:
 * ### common types:
 * > `VECTOR_CREATE(int)`
 * results in: empty vector
 *   
 * > `VECTOR_CREATE(int, 1, 3, 5)`
 * results in: [1, 2, 3]
 *   
 * > `VECTOR_CREATE(char, "string")`
 * results in: [`s`,`t`,`r`,`i`,`n`,`g`]
 * 
 *   
 * ### using arrays:
 * first define your type of array, ex: int3 for int[3]
 * ```
 * > typedef int int3[3] 
 * > VECTOR_TYPEDEF(int3)
 * > int3Vector v1 = VECTOR_CREATE(int[3], 1,2,3, 0,0,0)
 * // OR
 * > int3Vector v2 = VECTOR_CREATE(int[3], {1,2,3}, {0,0,0})
 * ```
 * results in: [(1,2,3), (0,0,0)]
 * 
 *     
 * ### using strings: 
 * ```
 * > typedef char char32[32];
 * > VECTOR_TYPEDEF(char32);
 * > char32Vector v = VECTOR_CREATE(char[32], "string 1", "string 2");
 * ```
 * results in: ["string 1", "string 2"]
 * 
 */
#define VECTOR_CREATE(type, ...) ({\
    __typeof__(type) v[] = {__VA_ARGS__};\
    vector_create(sizeof(type), sizeof(v)/sizeof(*v), v);\
})

/**
 * Creates a new vector with given size
 * all elements are set to zero
 * 
 * note: You must call `vector_delete()` later
 */
#define VECTOR_ALLOCATE(type, size)\
    vector_create(sizeof(type), size, 0)

/**
 * Push one or more values to vector's end
 * usage:
 * ```
 * > VECTOR_PUSHBACK(int_vector, 0)
 * > VECTOR_PUSHBACK(int_vector, 1, 2, 3)
 * 
 * > VECTOR_PUSHBACK(char_vector, "appended")
 * > VECTOR_PUSHBACK(char_vector, 'a', 'd', 'd')
 * 
 * > VECTOR_PUSHBACK(float_vector, 3.1, 9.21)
 * 
 * > VECTOR_PUSHBACK(char32_vector, "string 1", "string 2")
 * ```
 * see `VECTOR_CREATE` for `char32` explanation
 */
#define VECTOR_PUSHBACK(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    vector_pushback(vector, sizeof(v)/sizeof(*v), v);\
})

/**
 * Push one or more values to vector's end
 * usage:
 * ```
 * > VECTOR_PUSHFRONT(int_vector, 0)
 * > VECTOR_PUSHFRONT(int_vector, 1, 2, 3)
 * 
 * > VECTOR_PUSHFRONT(char_vector, "appended")
 * > VECTOR_PUSHFRONT(char_vector, 'a', 'd', 'd')
 * 
 * > VECTOR_PUSHFRONT(float_vector, 3.1, 9.21)
 * 
 * > VECTOR_PUSHFRONT(char32_vector, "string 1", "string 2")
 * ```
 * see `VECTOR_CREATE` for `char32` explanation
 * note: multiple values will be pushed in the same order
 */
#define VECTOR_PUSHFRONT(vector, ...) ({\
    __typeof__(*vector->at) v[] = {__VA_ARGS__};\
    vector_pushfront(vector, sizeof(v)/sizeof(*v), v);\
})

/**
 * Gets the last element
 * and remove it from vector
 */
#define VECTOR_POPBACK(vector) (*(__typeof__(vector->at))vector_popback(vector));

/**
 * Gets the first element
 * and remove it from vector
 */
#define VECTOR_POPFRONT(vector) (*(__typeof__(vector->at))vector_popfront(vector));


// ===== FUNCTIONS ===== //


// Create vector and pass values
void* vector_create(size_t data_size, size_t initial_size, void* initial_values);

// Push `data` to vector's end
void vector_pushback(AnyVector vector, size_t num_elements, void* data);

// Push `data` to vector's begin
void vector_pushfront(AnyVector vector, size_t num_elements, void* data);

// Pop vector's last element
void* vector_popback(AnyVector vector);

// Pop vector's first element
void* vector_popfront(AnyVector vector);

// Remove element at given index
void vector_remove(AnyVector vector, size_t index);

// Free it's internal array and structure 
void vector_delete(AnyVector vector);
