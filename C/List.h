/* ====  Made by Gabriel-AB  ====
 * Generic double linked List Library 
 */

#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

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


// ============ Library internal Functions ============ //
/* 
 * Allocate a node and return the pointer
 * obs: size in bytes
 */
struct node *_list_new_node(size_t size) {
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
void _list_del_node(struct node *n) {
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
void _list_del_all_nodes(struct node *n) {
    if (n){
        _list_del_all_nodes(n->next);
        free(n->data);
        free(n);
    }
}

/* 
 * Create a new node at List's tail
 * and return the data pointer
 */
void *_list_push_back(List *a) {
    struct node *n = _list_new_node(a->data_size);
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
 * and return the data pointer
 */
void *_list_push_front(List *a) {
    struct node *n = _list_new_node(a->data_size);
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
struct node *_list_pop_back(List *a) {
    if (a->size) {
        struct node *result = _list_new_node(a->data_size);
        result->data = a->tail->data;
        if (a->size == 1) {
            _list_del_node(a->tail);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->tail = a->tail->back;
            _list_del_node(a->tail->next);
            a->tail->next = NULL;
        }
        a->size--;
        return result;
    }
    return NULL;
}

/* 
 * Delete the first node of a list 
 * Obs: just copy out of list, the node must be freed later.
 */
struct node *_list_pop_front(List *a) {
    if (a->size > 0) {
        struct node *result = _list_new_node(a->data_size);
        result->data = a->head->data;
        if (a->size == 1) {
            _list_del_node(a->head);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->head = a->head->next;
            _list_del_node(a->head->back);
            a->head->back = NULL;
        }
        a->size--;
        return result;
    }
    return NULL;
}

/* 
 * Find the linked node by the given index
 */
struct node * _list_node_find(struct node * node, unsigned int index) {
    return (index) ? _list_node_find(node->next, index-1) : node;
}

/* 
 * Find a given index of list
 */
void * _list_at(List list, unsigned int index) {
    return _list_node_find(list.head, index)->data;
}

// ============ PUBLIC FUNCTIONS ============ //

/* 
 * ### Create a Empty List holding the type size
 * The 'type' argument is the type of data that will be placed on the list.
 * ex: float, int...
 * Obs: you must call ListDelete() to free alocated memory
 */
#define List_create(type) (List){NULL, NULL, 0, sizeof(type)}

/* 
 * ### Free all nodes from a Created List
 */
#define List_delete(List) _list_del_all_nodes(List.head)

/* 
 * ### Create a node in list's tail and atribute it's value.
 * Obs: if value is a structure passed by value,
 * you must wrap it with ()
 * Ex: ( (struct_type){values, ...} )
 */
#define List_pushBack(list, value) ({\
    void * data = _list_push_back(&list);\
    if (data) *(typeof(value)*)data = value;\
})

/* 
 * ### Create a node in list's head and atribute it's value.
 * Obs: if value is a structure passed by value,
 * you must wrap it with ()
 * Ex: ( (struct_type){values, ...} )
 */
#define List_pushFront(list, value) ({\
    void * data = _list_push_front(&list);\
    if (data) *(typeof(value)*)data = value;\
})

/* 
 * ### Delete the last node and return it's value.
 * Argument 'type' must be the same in the list Creation.
 */
#define List_popBack(type,list) ({\
    type res = {0};\
    struct node *n = _list_pop_back(&list);\
    if(n) res = *(type*)(n->data);\
    _list_del_node(n);\
    res;\
})

/* 
 * ### Delete the first node and return it's value.
 * Argument 'type' must be the same in the list Creation.
 */
#define List_popFront(type,list) ({\
    type res = {0};\
    struct node *n = _list_pop_front(&list);\
    if(n){\
        res = *(type*)(n->data);\
        _list_del_node(n);\
    }\
    res;\
})

/* 
 * ### Get element from given index
 * Argument 'type' must be the same in the list Creation.
 * ex: float, int...
 */
#define List_at(type, list, index) ({ *(type*)_list_at(list, index); })

/* 
 * ### Map a function through the list
 * Argument 'type' must be the same in the list Creation.
 */
#define List_map(type, list, function) ({\
    struct node * node = list.head;\
    while(node) {\
        *(type*)(node->data) = function(*(type*)(node->data));\
        node = node->next;\
    }\
})

/* 
 * ### Declare all functions for a type of list.
 * Executable size reduced and faster compilation.
 * No need to pass type anymore.
 */
#define List_declare(type) \
void ListPushBack(List list, type value) {List_pushBack(list, value);}\
void ListPushFront(List list, type value) {List_pushFront(list, value);}\
type ListPopBack(List list) { List_popBack(type, list);}\
type ListPopFront(List list) { List_popFront(type, list);}\
void ListMap(List list, type (*function)(type)) {List_map(type, list, function);}\
type ListAt(List list, int index) { return List_at(type, list, index);}

#endif // LIST_H