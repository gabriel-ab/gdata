#include "gdata/stack.h"
#include <stdlib.h>
#include <string.h>

AnyStack stack_create(size_t dsize, size_t initial_size, void *initial_values) {
    Stack *stack = malloc(sizeof(struct stack));
    stack->size = 0;
    stack->head = NULL;
    stack->internal.pop = NULL;
    *(size_t*)&stack->internal.dsize = dsize;
    if (initial_values)
        for (size_t i = 0; i < initial_size; i++)
            stack_push(stack, (char*)initial_values + i*dsize);
    return stack;
}

void stack_delete(AnyStack stack) {
    stack_clear(stack);
    free(stack);
}

// Push a new item to the head
void stack_push(AnyStack stack, void *data) {
    Stack *s = stack;
    struct stack_node* node = malloc(sizeof(struct stack_node*) + s->internal.dsize);
    if (node) {
        memcpy(node->data, data, s->internal.dsize);
        node->next = s->head;
        s->head = node;
        s->size++;
    }
}

// Pop the head
void* stack_pop(AnyStack stack) {
    Stack *s = stack;
    struct stack_node* node = s->head;
    s->head = node->next;
    s->size--;
    if (s->internal.pop)
        free(s->internal.pop);
    s->internal.pop = node;
    return node->data;
}

void* stack_at(const AnyStack stack, int index) {
    Stack *s = stack;
    struct stack_node *node = s->head;
    index = index - s->size + 1;
    while(index++ && node)
        node = node->next;
    return node->data;
}

void* stack_value(const AnyStack stack) {
    return stack_at(stack, 0);
}

void stack_to_array(const AnyStack stack, void* array) {
    Stack *s = stack;
    struct stack_node *node = s->head;
    size_t dsize = s->internal.dsize;
    size_t i = s->size;
    while (node) {
        memcpy(array + dsize * --i, node->data, dsize);
        node = node->next;
    }
}

AnyStack stack_copy(const AnyStack stack) {
    Stack *s = stack;
    char data[s->size*s->internal.dsize];
    stack_to_array(s, data);
    return stack_create(s->internal.dsize, s->size, data);
}

void stack_clear(AnyStack stack) {
    Stack *s = stack;
    while (s->size)
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

void stack_reverse(AnyStack stack) {
    Stack *s = stack;
    struct stack_node *nodes[s->size];
    nodes[0] = s->head;

    for (int i = 1; i < s->size; i++) {
        nodes[i] = nodes[i -1]->next;
    }
    for (int i = s->size -1; i > 0; i--) {
        nodes[i]->next = nodes[i -1];
    }
    nodes[0]->next = NULL;
    s->head = nodes[s->size -1];
}

bool stack_equals(const AnyStack a, const AnyStack b) {
    Stack *A = a, *B = b;
    if (A->size != B->size || A->internal.dsize != B->internal.dsize)
        return false;
    
    for (struct stack_node *an = A->head, *bn = B->head; an && bn; 
         an = an->next, bn = bn->next)
    {
        if (memcmp(an->data, bn->data, A->internal.dsize) != 0)
            return false;
    }
    return true;
}