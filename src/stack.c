#include "gdata/stack.h"
#include <stdlib.h>
#include <string.h>

Stack stack_create(size_t dsize, size_t initial_size, void *initial_values) {
    Stack stack = malloc(sizeof(struct stack));
    stack->size = 0;
    stack->head = NULL;
    stack->internal.pop = NULL;
    *(size_t*)&stack->internal.dsize = dsize;
    if (initial_values)
        for (size_t i = 0; i < initial_size; i++)
            stack_push(stack, (char*)initial_values + i*dsize);
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

static struct stack_node* _stack_reverse(struct stack_node* node, struct stack_node* prev) {
    struct stack_node* res;
    if (node) {
        res = _stack_reverse(node->next, node);
        node->next = prev;
    } else {
        res = prev;
    }
    return res;
}

void stack_reverse(Stack stack) {
    struct stack_node *nodes[stack->size];
    nodes[0] = stack->head;

    for (int i = 1; i < stack->size; i++) {
        nodes[i] = nodes[i -1]->next;
    }
    for (int i = stack->size -1; i > 0; i--) {
        nodes[i]->next = nodes[i -1];
    }
    nodes[0]->next = NULL;
    stack->head = nodes[stack->size -1];
}

bool stack_equals_data(Stack s, void* data) {
    struct stack_node *node = s->head;
    size_t i = 0;
    size_t dsize = s->internal.dsize;
    while (node != NULL) {
        void *a = node->data;
        void *b = data + i*dsize;
        if (memcmp(a,b, dsize) != 0)
            return false;
        node = node->next;
        i++;
    }
    return true;
}

bool stack_equals(Stack a, Stack b) {
    if (a->size != b->size || a->internal.dsize != b->internal.dsize)
        return false;
    
    for (struct stack_node *an = a->head, *bn = b->head; 
         an && bn; 
         an = an->next, bn = bn->next)
    {
        if (memcmp(an->data, bn->data, a->internal.dsize) != 0)
            return false;
    }
    return true;
}