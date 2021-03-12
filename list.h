/** 
 * Generic double linked List v1.7
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */

#pragma once
#include <stddef.h>

/**
 * ### Define a new type of List
 */
#define LIST_TYPEDEF(type)\
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

// Defining basic data lists
LIST_TYPEDEF(int);
LIST_TYPEDEF(char);
LIST_TYPEDEF(float);

// AnyList (intList, floatList, ...)
typedef void *AnyList;


// ===== MACROS ===== //


// get list's type witch is holded by internal.dtype
#define LIST_DTYPE(list) __typeof__(*list->internal.dtype)

/**
 * ## Push itens in the front of the list 
 * the type of data supported depends on the list type
 */
#define LIST_PUSHBACK(list, ...) ({\
    LIST_DTYPE(list) _args[] = {__VA_ARGS__};\
    list_pushback(list, sizeof(_args)/sizeof(*_args), _args);\
})

/**
 * Push itens in the front of the list
 * the type of data supported depends on the list type
 */
#define LIST_PUSHFRONT(list, ...) ({\
    LIST_DTYPE(list) _args[] = {__VA_ARGS__};\
    list_pushfront(list, sizeof(_args)/sizeof(*_args), _args);\
})

/**
 * Pop the element at the given index
 * if `index` is negative, searchs in reverse.
 */
#define LIST_POP(list, index) (*(LIST_DTYPE(list)*)list_pop(list, index))

/**
 * Pop list's tail 
 * returns: list's data type
 */
#define LIST_POPBACK(list) (*(LIST_DTYPE(list)*)list_pop(list, -1))

/**
 * Pop list's head 
 * returns: list's data type
 */
#define LIST_POPFRONT(list) (*(LIST_DTYPE(list)*)list_pop(list, 0))

/**
 * Pop the passed node, checking and updating list
 * returns: list's data type
 */
#define LIST_POPNODE(list, node) (*(LIST_DTYPE(list)*)list_pop_node(list,node))


/**
 * ## Get element at the given index
 * if negative, search in reverse
 */
#define LIST_AT(list, index) (*(LIST_DTYPE(list)*)list_at(list, index))

/** 
 * ## `for` wrapper for AnyList
 * cursor: variable name to use within the scope 
 * use `cursor->data` to list's data at current iteration
 */
#define LIST_FOR_EACH(cursor, list)\
    for (\
        __typeof__(struct {\
            void *next, *back;\
            LIST_DTYPE(list) data;\
        })\
        *cursor = (void*)list->head;\
        cursor != NULL;\
        cursor = cursor->next)


/**
 * Same as `list_push` but item is passed by value
 */
#define LIST_PUSH(list, index, item) list_push(list, index, ((LIST_DTYPE(list)[]){item}))

/** 
 * ### Create a list and push values if passed
 * 
 * sage:
 * * Ex: `LIST_CREATE(int)`
 * * Ex: `LIST_CREATE(double, 2.7, 3.2, 0.5, 0.1, 0.9)`
 * 
 * Obsevations:
 * * You must call `list_delete()` to free alocated memory
 */
#define LIST_CREATE(type, ...) ({\
    __typeof__(type) _args[] = {__VA_ARGS__};\
    (type##List)list_create(sizeof(type), sizeof(_args)/sizeof(*_args), _args);\
})


// ===== FUNCTIONS ===== //


/** 
 * ### Create a list
 * LIST_CREATE() may be simpler to use
 * 
 * Args:
 * * data_size: data size in bytes (all elements will allocate this size)
 * * initial_size: initial size of the list
 * * initial_values: pointer to data that will be pushed first
 */
void* list_create(size_t data_size, size_t initial_size, void *initial_values);

/**
 * Push `num_elements` of `data` to list's end.
 * obs: LIST_PUSHFRONT may be simpler to use
 */
void list_pushback(AnyList list, size_t num_elements, void *data);

/**
 * Push `num_elements` of `data` to list's begin.
 * obs: LIST_PUSHFRONT may be simpler to use
 */
void list_pushfront(AnyList list, size_t num_elements, void *data);

/** 
 * Push item in the index especified
 * if `index` is negative, searchs in reverse. 
 * obs: LIST_PUSH() may be simpler to use
 */
void list_push(AnyList _list, int index, void * item);

/**
 * Pop the element at index
 * if `index` is negative, searchs in reverse.
 * 
 * returns: value from deleted node
 * obs: LIST_POP() may be simpler to use
 */
void* list_pop(AnyList list, int index);

/**
 * Find a given index of list and return the data pointer
 * if index is negative, searchs in reverse.
 * obs: LIST_AT() may be simpler to use
 */
void* list_at(AnyList list, int index);

/**
 * Receive some node from list and remove it properly
 * 
 * returns: value from deleted node
 * obs: LIST_POP_NODE() may be simpler to use
 */
void* list_pop_node(AnyList _list, void* node);

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
