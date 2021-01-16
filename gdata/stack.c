#include "stack.h"
#include <string.h>

static void* returned;

__attribute__((constructor))
void stack_init() {
    // allocated to free at first call of _stack_pop()
    // this removes an 'if' check
    returned = malloc(1);
}

Stack _stack_create(size_t data_size, size_t size, void *data) {
    Stack stack = malloc(sizeof(struct stack));
    *(size_t*)&stack->data_size = data_size;
    if (data)
        for (size_t i = 0; i < size; i++)
            _stack_push(stack, data + i*data_size);
    return stack;
}

/* 
 * ### Push a new item to the head
 */
void _stack_push(Stack stack, void *data) {
    struct stack_node* node = calloc(1, sizeof(struct stack_node*) + stack->data_size);
    // copy data
    for (size_t i = 0; i < stack->data_size; i++)
        node->data[i] = ((char*)data)[i];

    node->next = stack->last;
    stack->last = node;
    stack->size++;
}

/* 
 * ### Pop the head
 * free the variable returned before the reattibution
 */
void* _stack_pop(Stack stack) {
    struct stack_node* node = stack->last;
    stack->last = node->next;
    stack->size--;
    free(returned);
    returned = node;
    return node->data;
}

void StackDelete(Stack stack) {
    while (stack->size)
        _stack_pop(stack);
    free(stack);
}
