/* 
 * Generic Stack 
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */
#pragma once
#include <stddef.h>
#include <stdbool.h>

#define STACK_TYPEDEF(type)\
typedef union type ## _stack {\
    struct {\
        size_t length;\
        struct {\
            const size_t dsize;\
            struct stack_node *pop;\
        } internal;\
        struct stack_node *head;\
    };\
    type dtype;\
} type ## Stack

/**
 * @brief Creates a new Stack and attribute some values if passed
 * 
 * @param type: any defined type. ex: int, float, etc...
 * @param __VA_ARGS__: values to initialize stack
 * 
 * ex:
 *   STACK_CREATE(float)
 *   STACK_CREATE(int, 1,2,3)
 */
#define STACK_CREATE(type, ...) ({\
    type _arr[] = {__VA_ARGS__};\
    stack_create(sizeof(type), sizeof(_arr)/sizeof(type), _arr);\
})

#define STACK_POP(stack) *(typeof(stack->dtype)*)stack_pop(stack)

#define STACK_PUSH(stack, item) stack_push(stack, (typeof(stack->dtype)){item})

#define STACK_AT(stack, index) (*(typeof(stack->dtype)*)stack_at(stack, index))


struct stack_node {
    struct stack_node *next;
    char data[];
};

typedef struct stack {
    size_t length;
    struct {
        const size_t dsize;
        struct stack_node *pop;
    } internal;
    struct stack_node *head;
} Stack;

// ===== FUNCTIONS ===== //


/**
 * @brief Create a new stack with values if passed
 * 
 * @param dsize: data size in bytes (all elements will allocate this size)
 * @param initial_size: initial size of the list. (0 is valid)
 * @param initial_values: pointer to data that will be pushed first. (0 is valid)
 */
void* stack_create(size_t dsize, size_t initial_size, void *initial_values);

// Free a created stack
void stack_delete(void* stack);

/**
 * @brief Push a new item into the stack
 * 
 * @param data reference to your data
 */
void stack_push(void* stack, void *data);

/// @brief Remove all elements
void stack_clear(void* stack);

/// @brief Reverse stack
void stack_reverse(void* stack);

/**
 * @brief Pop the last item from the stack
 * 
 * @return reference to value (will last until next call)
 */
void* stack_pop(void* stack);

/// @brief Get some value from stack. index must be in [0, stack->length)
void* stack_at(const void* stack, int index);

/// @brief Get the value on head
void* stack_value(const void* stack);

/// @brief Gather all data from stack to array
void stack_to_array(const void* stack, void* array);

/// @brief Create a copy of passed stack
void* stack_copy(const void* stack);

/// @brief Check equaliity between each element
bool stack_equals(const void* a, const void* b);