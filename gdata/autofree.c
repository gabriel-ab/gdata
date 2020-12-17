#include "autofree.h"

struct clean_stack {
    struct clean_stack *back;
    void * data;
} *garbage_collector;


__attribute__((constructor))
void init_garbage_collector() {
    garbage_collector = calloc(1,sizeof(struct clean_stack));
}

__attribute__((destructor))
void delete_garbage_collector() {
    struct clean_stack * n = garbage_collector->back;
    free(garbage_collector);
    
    while (n) {
        free(n->data);
        garbage_collector = n;
        n = n->back;
        free(garbage_collector);
    }
}

void* autofree(void * ptr) {
    garbage_collector->data = ptr;
    struct clean_stack *n = malloc(sizeof(struct clean_stack));
    n->back  = garbage_collector;
    garbage_collector = n;
    return ptr;
}

void autofreeCancel(void *ptr) {
    struct clean_stack *n, *next;
    n = garbage_collector;
    while (n) {
        if (n->data == ptr) {
            next->back = n->back;
            free(next);
        }
        next = n;
        n = n->back;
    }
}
