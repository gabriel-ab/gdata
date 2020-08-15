/*  ================  Made by Gabriel-AB  ================
 *  Simple List Library 
 *  To use your type in the list, you must define DATA_TYPE {your_type}
 *  before you include the library 
 */

#include <stdlib.h>
#ifndef LIST_H
#define LIST_H

// #define get(pointer,type) (*(type*)pointer)
#define cast(pointer,type) ((type*)pointer)

struct node {
    struct node *next;
    struct node *back;
    void *data;
};

//  ======== node Constructor/Destructor ==========
struct node *new_node(size_t data_size) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if (ptr) {
        ptr->back = NULL;
        ptr->next = NULL;
        ptr->data = malloc(data_size);
    }
    return ptr;
}
void del_node(struct node *n) {
    if (n) {
        if (n->back) n->back->next = n->next;
        if (n->next) n->next->back = n->back;
        free(n->data);
        free(n);
    }
}
// rescursive deletion of nodes
void internal_del_all_nodes(struct node *n) {
    if (n){
        internal_del_all_nodes(n->next);
        free(n->data);
        free(n);
    }
}

typedef struct {
    struct node *head;
    struct node *tail;
    size_t size;
    const size_t data_size;
}List;

// ======== List Functions ==========

// ======== List Constructor/Destructor ==========
/* 
 * List Create a Empty List holding the type size
 */
#define ListCreate(type) (List){NULL, NULL, 0, sizeof(type)}
// #define ListCreate(datatype, name) List name = (List){NULL, NULL, 0, sizeof(datatype)}
#define ListDelete(List) internal_del_all_nodes(List.head)

// ==== Push functions ==== //

// Assert that can create a new node at list´s tail and return a pointer of the data
void *internal_push_back(List *a) {
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
// Create a node in list´s tail and atribute a value
#define push_back(list, value) ({\
    void * data = internal_push_back(&list);\
    if (data)\
        set(data, typeof(value)) = value;\
})

// Assert that can create a new node at list´s head and return a pointer of the data
void *internal_push_front(List *a) {
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
// Push a value to head of a list
#define push_front(list, value) ({\
    void * data = internal_push_front(&list);\
    if (data)\
        set(data, typeof(value)) = value;\
})


// ==== Pop functions ==== //

// Assert that can delete the last node
struct node *internal_pop_back(List *a) {
    if (a->size > 0) {
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
// delete the last node and return it´s value
#define pop_back(type,list) ({\
    type res = {0};\
    struct node *n = internal_pop_back(&list);\
    if(n){\
        res = set(n->data,type);\
        del_node(n);\
    }\
    res;\
})

// Assert that can delete the first node
struct node *internal_pop_front(List *a) {
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
// delete the first node and return it´s value
#define pop_front(type,list) ({\
    type res = {0};\
    struct node *n = internal_pop_front(&list);\
    if(n){\
        res = set(n->data,type);\
        del_node(n);\
    }\
    res;\
})

typedef 

#endif // LIST_H