/* 
 * Generic Stack v1.3
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */
#pragma once
#include <stddef.h>

struct stack_node {
    struct stack_node *next;
    char data[];
};

typedef struct stack {
    struct stack_node *head;
    size_t size;
    struct {
        struct stack_node *pop;
        const size_t dsize;
    } internal;
} *Stack;



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


// ===== FUNCTIONS ===== //


/**
 * @brief Create a new stack with values if passed
 * 
 * @param dsize: data size in bytes (all elements will allocate this size)
 * @param initial_size: initial size of the list. (0 is valid)
 * @param initial_values: pointer to data that will be pushed first. (0 is valid)
 */
Stack stack_create(size_t dsize, size_t initial_size, void *initial_values);

// Free a created stack
void stack_delete(Stack stack);

/**
 * @brief Push a new item into the stack
 * 
 * @param data reference to your data
 */
void stack_push(Stack stack, void *data);

/**
 * @brief Pop the last item from the stack
 * 
 * @return reference to value (will last until next call)
 */
void* stack_pop(Stack stack);

/// @brief Remove all elements
void stack_clear(Stack stack);

/// @brief Reverse stack
void stack_reverse(Stack stack);