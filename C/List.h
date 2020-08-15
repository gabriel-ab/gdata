/*  ================  Made by Gabriel-AB  ================
 *  Simple List Library 
 *  To use your type in the list, you must define DATA_TYPE {your_type}
 *  before you include the library 
 */

#include <stdlib.h>
#ifndef LIST_H
#define LIST_H

struct node {
    struct node *next;
    struct node *back;
    void *data;
};

typedef struct {
    struct node *head;
    struct node *tail;
    size_t size;
    const size_t data_size;
}List;


//  ========= Internal Functions ==========

/* 
 * Allocate a node and return the pointer
 * obs: size in bytes
 */
struct node *new_node(size_t size) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if (ptr) {
        ptr->back = NULL;
        ptr->next = NULL;
        ptr->data = malloc(size);
    }
    return ptr;
}

/* 
 * Free the given node,
 * trying to attach back to next and next to back
 */
void del_node(struct node *n) {
    if (n) {
        if (n->back) n->back->next = n->next;
        if (n->next) n->next->back = n->back;
        free(n->data);
        free(n);
    }
}
/* 
 * Rescursive deletion of nodes
 */
void _internal_del_all_nodes(struct node *n) {
    if (n){
        _internal_del_all_nodes(n->next);
        free(n->data);
        free(n);
    }
}


// ============ Library internal Functions ============ //
/* 
 * Create a new node at List's tail
 */
void *_internal_push_back(List *a) {
    struct node *n = new_node(a->data_size);
    if (n == NULL) return NULL;
    n->next = NULL;
    n->back = a->tail;
    if (a->tail)
        a->tail->next = n;
    else
        a->head = n;
    a->tail = n;
    a->size++;
    return n->data;
}

/* 
 * Create a new node at List's head
 */
void *_internal_push_front(List *a) {
    struct node *n = new_node(a->data_size);
    if (n == NULL) return NULL;
    n->back = NULL;
    n->next = a->head;
    if (a->head)
        a->head->back = n;
    else
        a->tail = n;
    a->head = n;
    a->size++;
    return n->data;
}

/* 
 * Delete the last node of a list.
 */
struct node *_internal_pop_back(List *a) {
    if (a->size) {
        struct node *result = new_node(a->data_size);
        result->data = a->tail->data;
        if (a->size == 1) {
            del_node(a->tail);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->tail = a->tail->back;
            del_node(a->tail->next);
            a->tail->next = NULL;
        }
        a->size--;
        return result;
    }
    return NULL;
}

/* 
 * Delete the first node of a list,
 */
struct node *_internal_pop_front(List *a) {
    if (a->size > 0) {
        struct node *result = new_node(a->data_size);
        result->data = a->head->data;
        if (a->size == 1) {
            del_node(a->head);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->head = a->head->next;
            del_node(a->head->back);
            a->head->back = NULL;
        }
        a->size--;
        return result;
    }
    return NULL;
}


// ============ FUNCTIONS DECLARATIONS ============ //
/* 
 * List Create a Empty List holding the type size
 * obs: you must call ListDelete() to free alocated memory
 */
#define ListCreate(type) (List){NULL, NULL, 0, sizeof(type)}

/* 
 *  Free all nodes from a Created List
 */
#define ListDelete(List) internal_del_all_nodes(List.head)

/* 
 * Create a node in list´s tail and atribute value.
 */
#define List_pushBack(list, value) ({\
    void * data = _internal_push_back(&list);\
    if (data) *(typeof(value)*)data = value;\
})

/* 
 * Create a node in list´s head and atribute value.
 */
#define List_pushBack(list, value) ({\
    void * data = _internal_push_front(&list);\
    if (data) *(typeof(value)*)data = value;\
})

/* 
 * Delete the last node and return it´s value.
 */
#define List_popBack(type,list) ({\
    type res = {0};\
    struct node *n = _internal_pop_back(&list);\
    if(n) res = *(type*)(n->data);\
    del_node(n);\
    res;\
})

/* 
 * Delete the first node and return it´s value.
 */
#define List_popFront(type,list) ({\
    type res = {0};\
    struct node *n = _internal_pop_front(&list);\
    if(n){\
        res = *(type*)(n->data);\
        del_node(n);\
    }\
    res;\
})

#endif // LIST_H