#include "stack.h"
#include <string.h>

Stack _stack_create(size_t data_size, size_t size, void *data) {
    Stack stack = malloc(sizeof(struct stack));
    *(size_t*)&stack->internal.data_size = data_size;
    if (data)
        for (size_t i = 0; i < size; i++)
            stack_push(stack, data + i*data_size);
    return stack;
}

void stack_delete(Stack stack) {
    stack_clear(stack);
    free(stack);
}

// Push a new item to the head
void stack_push(Stack stack, void *data) {
    struct stack_node* node = calloc(1, sizeof(struct stack_node*) + stack->internal.data_size);
    memcpy(node->data, data, stack->internal.data_size);
    node->next = stack->head;
    stack->head = node;
    stack->size++;
}

// Pop the head
void* stack_pop(Stack stack) {
    struct stack_node* node = stack->head;
    stack->head = node->next;
    stack->size--;
    if (stack->internal.pop)
        free(stack->internal.pop);
    stack->internal.pop = node;
    return node->data;
}

void stack_clear(Stack stack) {
    while (stack->size)
        stack_pop(stack);
    free(stack->internal.pop);
    stack->internal.pop = NULL;
}