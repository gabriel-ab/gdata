/* 
 * Generic Stack v1.3
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */
#pragma once
#include <stddef.h>
#include <stdbool.h>

#define STACK_TYPEDEF(type)\
typedef union stack_##type {\
    struct {\
        size_t size;\
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

#define STACK_AT(stack, index) (*(typeof(stack->dtype)*)stack_at(stack, index))


struct stack_node {
    struct stack_node *next;
    char data[];
};

typedef struct stack {
    size_t size;
    struct {
        const size_t dsize;
        struct stack_node *pop;
    } internal;
    struct stack_node *head;
} Stack;

typedef void* AnyStack;

// ===== FUNCTIONS ===== //


/**
 * @brief Create a new stack with values if passed
 * 
 * @param dsize: data size in bytes (all elements will allocate this size)
 * @param initial_size: initial size of the list. (0 is valid)
 * @param initial_values: pointer to data that will be pushed first. (0 is valid)
 */
AnyStack stack_create(size_t dsize, size_t initial_size, void *initial_values);

// Free a created stack
void stack_delete(AnyStack stack);

/**
 * @brief Push a new item into the stack
 * 
 * @param data reference to your data
 */
void stack_push(AnyStack stack, void *data);

/// @brief Remove all elements
void stack_clear(AnyStack stack);

/// @brief Reverse stack
void stack_reverse(AnyStack stack);

/**
 * @brief Pop the last item from the stack
 * 
 * @return reference to value (will last until next call)
 */
void* stack_pop(AnyStack stack);

/// @brief Get some value from stack. index must be in [0, stack->size)
void* stack_at(const AnyStack stack, int index);

/// @brief Get the value on head
void* stack_value(const AnyStack stack);

/// @brief Gather all data from stack to array
void stack_to_array(const AnyStack stack, void* array);

/// @brief Just copy
AnyStack stack_copy(const AnyStack stack);

/// @brief Check equaliity between each element
bool stack_equals(const AnyStack a, const AnyStack b);