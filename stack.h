/* 
 * Generic Stack v1.2
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */
#pragma once
#include <stdlib.h>

struct stack_node {
    struct stack_node *next;
    char data[];
};

typedef struct stack {
    struct stack_node *head;
    size_t size;
    struct {
        struct stack_node *pop;
        const size_t data_size;
    } internal;
} *Stack;


Stack _stack_create(size_t data_size, size_t size, void *data);

/* ### Creates a new Stack and attribute some values if passed
 * pass values separated by comma (,)
 * ex:
 *   stack_create(float)
 *   stack_create(int, 1,2,3)
 */
#define stack_create(type, values...) ({\
    type arr[] = {values};\
    _stack_create(sizeof(type), sizeof(arr)/sizeof(type), arr);\
})

// Free a created stack
void stack_delete(Stack stack);

/* ### Push a new item into the stack
 * 
 * usage example:
 * > stack_push(stack, (int[]){32}) // literal
 * 
 * > int myInt = 32;
 * > stack_push(stack, &myInt)      // variable
 */
void stack_push(Stack stack, void *data);

/* ### Pop the last item from the stack
 * aways deference it
 * ex: `int value = *(int*)stack_pop(stack);`
 */
void* stack_pop(Stack stack);

// ### Remove all elements
void stack_clear(Stack stack);