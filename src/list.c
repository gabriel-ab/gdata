#include "list.h"
#include <stdlib.h>
#include <string.h>

/* 
 * Allocate a list_node and return the pointer
 * obs: size in bytes
 */
static struct list_node *_list_new_node(size_t size) {
    return calloc(1,sizeof(struct list_node) + size);
}

/* 
 * Find the linked node by the given index.
 */
static struct list_node * _list_find(struct list_node * node, int index) {
    if (index < 0) {
        while (++index && node)
            node = node->back;
    } else {
        while (index-- && node)
            node = node->next;
    }
    return node;
}

/* 
 * List* constrained index finder 
 * if index is negative, searchs in reverse. 
 * return: the node
 */
static struct list_node * _list_node_at(List* list, int index) {
    return _list_find((index < 0) ? list->tail : list->head, index);
}


void * list_at(void* list, int index) {
    return _list_node_at(list, index)->data;
}

// ### Constructor
void* list_create(size_t dsize, size_t initial_size, void * initial_values) {
    List* list = calloc(1,sizeof(*list));
    *(size_t*)&list->internal.dsize = dsize;
    if (initial_values)
        list_pushback(list, initial_size, initial_values);
    else if (initial_size)
        list_resize(list, initial_size);
    return list;
}

// Push value in list's end.
void list_pushback(void* list, size_t num_elements, void *data) {
    List* L = list;
    while (num_elements--) {
        struct list_node *new_node = _list_new_node(L->internal.dsize);

        if (data) {
            memcpy(new_node->data, data, L->internal.dsize);
            data = (char*)data + L->internal.dsize;
        }

        if (L->length++ > 0) {
            L->tail->next = new_node;
            new_node->back = L->tail;
            L->tail = new_node;
        }
        else L->head = L->tail = new_node;
    }
}

// Push value in list's begin.
void list_pushfront(void* list, size_t num_elements, void * data) {
    List* L = list;
    while (num_elements--) {
        struct list_node *new_node = _list_new_node(L->internal.dsize);

        if (data) {
            void* curr = (char*)data + num_elements*L->internal.dsize;
            memcpy(new_node->data, curr, L->internal.dsize);
        }

        if (L->length++ > 0) {
            L->head->back = new_node;
            new_node->next = L->head;
            L->head = new_node;
        }
        else L->head = L->tail = new_node;
    }
}

//Push value in the index especified
void list_push(void* list, int index, void * item) {
    List* L = list;
    struct list_node *old_node = _list_node_at(list, index);
    if (old_node == NULL) return;

    struct list_node *new_node = _list_new_node(L->internal.dsize);
    if (new_node == NULL) return;

    // Goes before old item
    if (index >= 0) {
        new_node->next = old_node;
        new_node->back = old_node->back;
        if (old_node == L->head)
            L->head = new_node;
        else
            old_node->back->next = new_node;
        old_node->back = new_node;

    // Goes after old item
    } else {
        new_node->back = old_node;
        new_node->next = old_node->next;
        if (old_node == L->tail)
            L->tail = new_node;
        else
            old_node->next->back = new_node;
        old_node->next = new_node;
    }
    if (item) memcpy(new_node->data, item, L->internal.dsize);
    L->length++;
}

// Retrieve the in the index specified
void *list_pop_node(void* list, void* _node) {
    List* L = list;
    struct list_node* node = _node;

    if (node == L->tail)
        L->tail = node->back;
    if (node == L->head)
        L->head = node->next;
    L->length--;

    if (L->internal.pop)
        free(L->internal.pop);

    if (node->back) node->back->next = node->next;
    if (node->next) node->next->back = node->back;
    L->internal.pop = node;
    
    return node->data;
}

void* list_pop(void* list, int index) {
    return list_pop_node(list, _list_node_at(list, index));
}

// Resize a list allocating new memory,
void list_resize(void* list, unsigned int new_size) {
    List* L = list;
    if (L->length < new_size) {
        int count = new_size -L->length;
        list_pushback(L, count, NULL);
    } else {
        int count = L->length -new_size;
        for (int i = 0; i < count; i++)
            list_pop(L, -1);
    }
}

void* list_copy(void* list) {
    List *L = list;
    List *result = list_create(L->internal.dsize, 0, 0);
    struct list_node* node = L->head;
    while (node) {
        list_pushback(result, 1, node->data);
        node = node->next;
    }
    return result;
}

void list_clear(void* list) {
    List* L = list;
    struct list_node *node = L->head, *next;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
    if (L->internal.pop)
        free(L->internal.pop);
    L->internal.pop = L->head = L->tail = NULL;
    L->length = 0;
}

// convert to a array
void list_to_array(void* list, void* result) {
    List* L = list;
    struct list_node *n = L->head;
    for (size_t i = 0; i < L->length; i++) {
        void * array_element = (char*)result + i*L->internal.dsize;
        memcpy(array_element, n->data, L->internal.dsize);
        n = n->next;
    }
}

void list_delete(void* list) {
    list_clear(list);
    free(list);
}

void* list_slice(void* list, unsigned int begin, unsigned int end) {
    List* L = list;
    List* result = list_create(L->internal.dsize, 0, 0);
    struct list_node* node = L->head;

    end -= begin;
    while (begin--)
        node = node->next;

    while (end-- && node) {
        list_pushback(result, 1, node->data);
        node = node->next;
    }
    return result;
}

bool list_equals(void* a, void* b) {
    List *A = a, *B = b;
    if (A->length != B->length || A->internal.dsize != B->internal.dsize)
        return false;
    for (struct list_node *an = A->head, *bn = B->head;
         an != NULL;
         an = an->next, bn = bn->next)
    {
        if (memcmp(an->data, bn->data, A->internal.dsize) != 0)
            return false;
    }
    return true;
}