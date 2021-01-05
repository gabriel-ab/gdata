#include "stack.h"
#include <string.h>

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

    node->next = stack->head;
    stack->head = node;
    stack->size++;
}

/* 
 * ### Pop the head
 */
struct stack_node* _stack_pop(Stack stack) {
    struct stack_node* node = stack->head;
    stack->head = node->next;
    stack->size--;
    return node;
}

void StackDelete(Stack stack) {
    while (stack->size)
        free(_stack_pop(stack));
    free(stack);
}

int _stack_find(Stack stack, void *data) {
    struct stack_node* node = stack->head;
    int i = stack->size -1;
    while(node) {
        if (memcmp(node->data, data, stack->data_size) == 0)
            return i;
        node = node->next;
        i--;
    }
    return -1;
}