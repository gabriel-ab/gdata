/** 
 * Generic double linked List v1.6
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */

#pragma once
#include <stddef.h>


// Generic List
struct list_node {
    struct list_node* next;
    struct list_node* back;
    char data[];
};

typedef struct list {
    struct list_node* head;
    struct list_node* tail;
    size_t size;
    struct {
        struct list_node* pop;
        const size_t dsize;
    } internal;
} *List;

/**
 * ### Define a type specific list
 */
#define list_typedef(type)\
typedef struct type ## list {\
    struct list_node* head;\
    struct list_node* tail;\
    size_t size;\
    struct {\
        struct list_node* pop;\
        const size_t dsize;\
        type dtype[];\
    } internal;\
} *type ## List

typedef void *AnyList, *AnyListNode;

// get list of type
#define list(type) type##List

// get list type witch is holded by internal.dtype
#define list_dtype(list) __typeof__(*list->internal.dtype)

// Push itens in the front of the list
// the type of data supported depends on the list type
#define list_pushback(list, ...) ({\
    list_dtype(list) v[] = {__VA_ARGS__};\
    _list_pushback(list, sizeof(v)/sizeof(*v), v);\
})

// Push itens in the front of the list
// the type of data supported depends on the list type
#define list_pushfront(list, ...) ({\
    list_dtype(list) v[] = {__VA_ARGS__};\
    _list_pushfront(list, sizeof(v)/sizeof(*v), v);\
})

/**
 * Pop the element at the given index
 * if `index` is negative, searchs in reverse.
 */
#define list_pop(list, index) (*(list_dtype(list)*)_list_pop(list, index))

// Pop list's tail
#define list_popback(list) (*(list_dtype(list)*)_list_pop(list, -1))

// Pop list's head
#define list_popfront(list) (*(list_dtype(list)*)_list_pop(list, 0))

// Pop the passed node checking and updating list
#define list_popnode(list, node) (*(list_dtype(list)*)_list_pop_node(list,node))

// Get element at the given index
#define list_at(list, index) (*(list_dtype(list)*)_list_at(list, index))

/** 
 * ## for wrapper for AnyList
 * cursor: variable name. ex: 'node', 'current', 'i'... you name it
 * list: any kind of List. ex: List, intList, floatList...
 */
#define list_for_each(cursor, list)\
    for (__typeof__(struct { void *next, *back; list_dtype(list) data;})\
         *cursor = (void*)list->head; cursor; cursor = cursor->next)

#define list_push(list, index, item) _list_push(list, index, ((list_dtype(list)[]){item}))

/** 
 * ### Create a list and push values if passed
 * 
 * sage:
 * * Ex: `list_create(int)`
 * * Ex: `list_create(double, 2.7, 3.2, 0.5, 0.1, 0.9)`
 * 
 * Obsevations:
 * * You must call `list_delete()` to free alocated memory
 */
#define list_create(type, ...) ({\
    __typeof__(type) v[] = {__VA_ARGS__};\
    (type##List)_list_create(sizeof(type), sizeof(v)/sizeof(*v), v);\
})

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

/** 
 * ### Create a list
 * list_create() may be simpler to use
 * 
 * Args:
 * * data_size: data size in bytes (all elements will allocate this size)
 * * initial_size: initial size of the list
 * * initial_values: pointer to data that will be pushed first
 */
void* _list_create(size_t data_size, size_t initial_size, void *initial_values);

// Push `num_elements` of `data` to list's end.
void _list_pushback(AnyList list, size_t num_elements, void *data);

// Push `num_elements` of `data` to list's begin.
void _list_pushfront(AnyList list, size_t num_elements, void *data);

/** 
 * Push item in the index especified
 * if `index` is negative, searchs in reverse. 
 */
void _list_push(AnyList _list, int index, void * item);

/**
 * Pop the element at index
 * if `index` is negative, searchs in reverse.
 */
void* _list_pop(AnyList list, int index);

/**
 * Find a given index of list and return the data pointer
 * if index is negative, searchs in reverse. 
 */
void* _list_at(AnyList list, int index);

// Receive some node from list and remove it properly
void* _list_pop_node(AnyList _list, AnyListNode node);


// =========================== PUBLIC FUNCTIONS =========================== //

// Resize a list allocating new memory,
void list_resize(AnyList list, unsigned int new_size);

// Copy all content of src list
AnyList list_copy(AnyList src);

// Create a sub list using the passed interval [begin, end)
AnyList list_sublist(AnyList list, unsigned int begin, unsigned int end);

// Deletes all nodes and clear the list
void list_clear(AnyList list);

// Copy values to a array. (be sure to have suficient space in the array)
void list_to_array(AnyList list, void* result);

// free allocated memory
void list_delete(AnyList list);


// Defining basic data list 
list_typedef(int);
list_typedef(float);