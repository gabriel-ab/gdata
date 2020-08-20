/* ====  Made by Gabriel-AB  ====
 * Generic double linked List Library 1.0
 * 
 * Define LIST_DEBUG before inclusion to receive error messages.
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
    size_t data_size; // don't change before ListDelete()
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
 * trying to attach back to next and next to back.
 */
void _list_del_node(struct node *n, _Bool delete_data) {
    if (n) {
        if (n->back) n->back->next = n->next;
        if (n->next) n->next->back = n->back;
        if (delete_data) free(n->data);
        free(n);
    }
}

/* 
 * Rescursive deletion of nodes, and data.
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
void* _list_push_back(List *a) {
    struct node *n = _list_new_node(a->data_size);
    if (n) {
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
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: Failed to create node at list's tail");
#endif
    return NULL;
}

/* 
 * Create a new node at List's head 
 * and return the data pointer
 */
void* _list_push_front(List *a) {
    struct node *n = _list_new_node(a->data_size);
    if (n) {
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
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: Failed to create node at list's head");
#endif
    return NULL;
}

/* 
 * Delete the last node of a list, and return a the data pointer.
 * Obs: Data pointer must be freed later.
 */
void* _list_pop_back(List *a) {
    if (a->size) {
        void * data = a->tail->data;
        if (a->size == 1) {
            _list_del_node(a->tail, 0);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->tail = a->tail->back;
            _list_del_node(a->tail->next, 0);
            a->tail->next = NULL;
        }
        a->size--;
        return data;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: List is empty");
#endif
    return NULL;
}

/* 
 * Delete the first node of a list, and return a the data pointer.
 * Obs: Data pointer must be freed later.
 */
void* _list_pop_front(List *a) {
    if (a->size > 0) {
        void* result = a->head->data;
        if (a->size == 1) {
            _list_del_node(a->head, 0);
            a->head = NULL;
            a->tail = NULL;
        } else {
            a->head = a->head->next;
            _list_del_node(a->head->back, 0);
            a->head->back = NULL;
        }
        a->size--;
        return result;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: List is empty");
#endif
    return NULL;
}

/* 
 * Find the linked node by the given index.
 */
struct node * _list_node_find(struct node * node, unsigned int index) {
    return (index) ? _list_node_find(node->next, index-1) : node;
}

/* 
 * Find a given index of list and return the data pointer
 */
void * _list_at(List list, unsigned int index) {
    if (index < list.size) {
        return _list_node_find(list.head, index)->data;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: Index out of list!");
#endif
    return NULL;
}

// ============ PUBLIC FUNCTIONS ============ //

// # LIST_MACROS
/* 
 * ### Resize a list allocating new memory,
 * Obs: data might have garbage
 */
#define List_resize(list, new_size) ({\
    if (list.size < new_size)\
        for (int i = 0, count = new_size -list.size; i < count; i++)\
            _list_push_back(&list);\
    else\
        for (int i = 0, count = list.size -new_size ; i < count; i++)\
            free(_list_pop_back(&list));\
})

/* 
 * ### Create a node in list's tail and atribute it's value.
 * Obs: if value is a anonymous struct initializer,
 * you must wrap it with ()
 * Ex: ( (struct){values, ...} )
 */
#define List_pushBack(type, list, value) ({\
    if(sizeof(type) == list.data_size){\
        void* data = _list_push_back(&list);\
        if (data) *(type*)data = (type)value;\
    }\
})

/* 
 * ### Create a node in list's head and atribute it's value.
 * Obs: if value is a anonymous struct initializer,
 * you must wrap it with ()
 * Ex: ( (struct){values, ...} )
 */
#define List_pushFront(type, list, value) ({\
    if (sizeof(type) == list.data_size){\
        void* data = _list_push_front(&list);\
        if (data) *(type*)data = (type)value;\
    }\
})

/* 
 * ### Delete the last node and return it's value.
 * Argument 'type' must be the same in the list Creation.
 */
#define List_popBack(type,list) ({\
    void* data = _list_pop_back(&list);\
    type res = (data) ? *(type*)data : (type){0};\
    free(data);\
    res;\
})

/* 
 * ### Delete the first node and return it's value.
 * Argument 'type' must be the same in the list Creation.
 */
#define List_popFront(type,list) ({\
    void* data = _list_pop_front(&list);\
    type res = (data) ? *(type*)data : (type){0};\
    free(data);\
    res;\
})

/* 
 * ### Get element from given index
 * Argument 'type' must be the same in the list Creation.
 * ex: float, int...
 */
#define List_at(type, list, index) *(type*)_list_at(list, index)

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
 * ### Map a function through the list
 * The function must take data by reference
 * Argument 'type' must be the same in the list Creation.
 */
#define List_mapByRef(type, list, function) ({\
    struct node * node = list.head;\
    while(node) {\
        function((type*)(node->data));\
        node = node->next;\
    }\
})

/* 
 * ### Concatenate a Array on list's tail
 * Argument 'type' must be the same in the list Creation.
 * 'count' is array size
 */
#define List_pushArray(type, list, count, array...) ({\
    type* arr = array;\
    for (int i = 0; i < count; i++)\
        List_pushBack(type, list, arr[i]);\
})

/* 
 * ### Resizes the list to array size, then copy all values
 * Argument 'type' must be the same in the list Creation.
 * 'count' is array size
 */
#define List_copyArray(type, list, count, array...) ({\
    if (list.size != count)\
        List_resize(list, count);\
    type* arr = array;\
    for (int i = 0; i < count; i++)\
        List_at(type,list,i) = arr[i];\
})

/* 
 * ### Copy all content of src_list to dest_list
 * The list type must be the same.
 */
void ListCopy(List *dest_list, List *src_list) {
    if (src_list->data_size != dest_list->data_size) {
#ifdef LIST_DEBUG
        perror("LIST_DEBUG: Can't copy diferent type lists");
#endif
        return;
    }
    List_resize((*dest_list), src_list->size);

    struct node *d_node = dest_list->head, *s_node = src_list->head;
    while (d_node != NULL) {
        char *dst = (char*)d_node->data;
        char *src = (char*)s_node->data;
        for (size_t i = 0; i < src_list->data_size; i++) dst[i] = src[i];
        d_node = d_node->next;
        s_node = s_node->next;
    }
}

/* ## Declare some functions for a type of list.
 * 
 * Use it if calling many times the same function.
 * 
 * Executable size reduced and faster compilation.
 * No need to pass type anymore.
 * ## if You are looking for documentation, see #LIST_MACROS
 * 
 * #### Rename the functions as you prefer:
 * Obs: You can left empty.
 * prefix: goes before function name
 * suffix: goes after function name
 * 
 */
#define List_declare_functions(type, prefix, suffix)\
\
void prefix##List_pushBack##suffix(List *list, type value) {\
    List_pushBack(type, (*list), value);\
}\
\
void prefix##List_pushFront##suffix(List *list, type value) {\
    List_pushFront(type, (*list), value);\
}\
\
type prefix##List_popBack##suffix(List *list) {\
     return List_popBack(type, (*list));\
}\
\
type prefix##List_popFront##suffix(List *list) {\
     return List_popFront(type, (*list));\
}\
\
void prefix##List_map##suffix(List list, type (*function)(type)) {\
    List_map(type, list, function);\
}\
\
type prefix##List_at##suffix(List list, int index) {\
     return List_at(type, list, index);\
}\
void prefix##List_mapByRef##suffix(List list, void (*function)(type*)) {\
    List_mapByRef(type, list, function);\
}\
void prefix##List_pushArray##suffix(List *list, size_t count, type *array) {\
    List_pushArray(type, (*list), count, array);\
}\
void prefix##List_copyArray##suffix(List *list, size_t count, type *array) {\
    List_copyArray(type, (*list), count, array);\
}\


// ### CONSTRUCTORS
/* 
 * ### Create a Empty List holding the type size
 * The 'type' argument is the type of data that will be placed on the list.
 * ex: float, int...
 * Obs: you must call ListDelete() to free alocated memory
 */
#define ListCreate(type) (List){NULL, NULL, 0, sizeof(type)}

/* 
 * ### Create a List with a custom data_size.
 */
List ListCreateFromSize(size_t data_size) { return (List){NULL, NULL, 0, data_size}; }

/* 
 * ### Create a new copied List
 */
List ListCreateFromList(List input) {
    List res = {NULL, NULL, 0, input.data_size};
    ListCopy(&res, &input);
    return res;
}

// ### DESTRUCTORS
/* 
 * ### Free all nodes from a Created List, and clean it.
 */
#define ListDelete(list) ({\
    _list_del_all_nodes(list.head);\
    list.head = NULL;\
    list.tail = NULL;\
    list.size = 0;\
})

#endif // LIST_H