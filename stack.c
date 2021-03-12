#include "stack.h"
#include <stdlib.h>
#include <string.h>

Stack stack_create(size_t data_size, size_t initial_size, void *initial_values) {
    Stack stack = malloc(sizeof(struct stack));
    *(size_t*)&stack->internal.dsize = data_size;
    if (initial_values)
        for (size_t i = 0; i < initial_size; i++)
            stack_push(stack, (char*)initial_values + i*data_size);
    return stack;
}

void stack_delete(Stack stack) {
    stack_clear(stack);
    free(stack);
}

// Push a new item to the head
void stack_push(Stack stack, void *data) {
    struct stack_node* node = malloc(sizeof(struct stack_node*) + stack->internal.dsize);
    if (node) {
        memcpy(node->data, data, stack->internal.dsize);
        node->next = stack->head;
        stack->head = node;
        stack->size++;
    }
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

Stack stack_reverse(Stack stack) {
    Stack result = stack_create(stack->internal.dsize, 0, 0);
    while (stack->size)
        stack_push(result, stack_pop(stack));
    return result;
}