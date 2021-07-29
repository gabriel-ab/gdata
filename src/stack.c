#include "stack.h"
#include <stdlib.h>
#include <string.h>

void* stack_create(size_t dsize, size_t initial_size, void *initial_values) {
    Stack res = {.internal.dsize=dsize};
    Stack *stack = calloc(1, sizeof(struct stack));
    *(size_t*)&stack->internal.dsize = dsize;
    if (initial_values)
        for (size_t i = 0; i < initial_size; i++)
            stack_push(stack, (char*)initial_values + i*dsize);
    return stack;
}

void stack_delete(void* stack) {
    stack_clear(stack);
    free(stack);
}

// Push a new item to the head
void stack_push(void* stack, void *data) {
    Stack *s = stack;
    struct stack_node* node = malloc(sizeof(struct stack_node*) + s->internal.dsize);
    if (node) {
        memcpy(node->data, data, s->internal.dsize);
        node->next = s->head;
        s->head = node;
        s->length++;
    }
}

// Pop the head
void* stack_pop(void* stack) {
    Stack *s = stack;
    struct stack_node* node = s->head;
    s->head = node->next;
    s->length--;
    if (s->internal.pop)
        free(s->internal.pop);
    s->internal.pop = node;
    return node->data;
}

void* stack_at(const void* stack, int index) {
    const Stack *s = stack;
    struct stack_node *node = s->head;
    index = index - s->length + 1;
    while(index++ && node)
        node = node->next;
    return node->data;
}

void* stack_value(const void* stack) {
    return stack_at(stack, 0);
}

void stack_to_array(const void* stack, void* array) {
    const Stack *s = stack;
    struct stack_node *node = s->head;
    size_t dsize = s->internal.dsize;
    size_t i = s->length;
    while (node) {
        memcpy(array + dsize * --i, node->data, dsize);
        node = node->next;
    }
}

void* stack_copy(const void* stack) {
    const Stack *s = stack;
    char data[s->length*s->internal.dsize];
    stack_to_array(s, data);
    return stack_create(s->internal.dsize, s->length, data);
}

void stack_clear(void* stack) {
    Stack *s = stack;
    while (s->length)
        stack_pop(s);
    free(s->internal.pop);
    s->internal.pop = NULL;
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

void stack_reverse(void* stack) {
    Stack *s = stack;
    struct stack_node *nodes[s->length];
    nodes[0] = s->head;

    for (int i = 1; i < s->length; i++) {
        nodes[i] = nodes[i -1]->next;
    }
    for (int i = s->length -1; i > 0; i--) {
        nodes[i]->next = nodes[i -1];
    }
    nodes[0]->next = NULL;
    s->head = nodes[s->length -1];
}

bool stack_equals(const void* a, const void* b) {
    const Stack *A = a, *B = b;
    if (A->length != B->length || A->internal.dsize != B->internal.dsize)
        return false;
    
    for (struct stack_node *an = A->head, *bn = B->head; an && bn; 
         an = an->next, bn = bn->next)
    {
        if (memcmp(an->data, bn->data, A->internal.dsize) != 0)
            return false;
    }
    return true;
}