/** 
 * Generic List
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 * Macro functions inputs and outputs the type defined inside the list
 * held by `dtype` in lists defined with LIST_TYPEDEF
 * 
 */

#pragma once
#include <stddef.h>
#include <stdbool.h>

// ===== MACROS ===== //

/**
 * @brief Define a new type of List
 */
#define LIST_TYPEDEF(type)\
struct type##_list_node {\
    struct type##_list_node* next;\
    struct type##_list_node* back;\
    type data;\
};\
typedef struct type ## _list {\
    size_t length;\
    struct type##_list_node* head;\
    struct type##_list_node* tail;\
    struct {\
        struct type##_list_node* pop;\
        const size_t dsize;\
    } internal;\
} *type ## List

// get list's type witch is holded by internal.dtype
#define LIST_DTYPE(list) typeof((list)->head->data)

/**
 * @brief Push itens at back of the list.
 * The type of data supported depends on the list type
 * 
 * @param list: Any type of list. 
 * @param __VA_ARGS__: sequence of values to push.
 * 
 * @ex: intList -> LIST_PUSHBACK(list, 1, 2, ...)
 */
#define LIST_PUSHBACK(list, ...) ({\
    LIST_DTYPE(list) _args[] = {__VA_ARGS__};\
    list_pushback(list, sizeof(_args)/sizeof(*_args), _args);\
})

/**
 * @brief Push itens in the front of the list
 * the type of data supported depends on the list type
 * 
 * @param list: Any type of list. 
 * @param __VA_ARGS__: sequence of values to push.
 * 
 * @ex: intList -> LIST_PUSHBACK(list, 1, 2, ...)
 */
#define LIST_PUSHFRONT(list, ...) ({\
    LIST_DTYPE(list) _args[] = {__VA_ARGS__};\
    list_pushfront(list, sizeof(_args)/sizeof(*_args), _args);\
})

/**
 * @brief Pop the element at the given index.
 * if index is negative, searchs in reverse.
 */
#define LIST_POP(list, index) (*(LIST_DTYPE(list)*)list_pop(list, index))

/**
 * @brief Pop list's tail 
 * @returns list's internal data type. ex: intList -> int
 */
#define LIST_POPBACK(list) (*(LIST_DTYPE(list)*)list_pop(list, -1))

/**
 * @brief Pop list's head 
 * @returns list's internal data type. ex: intList -> int
 */
#define LIST_POPFRONT(list) (*(LIST_DTYPE(list)*)list_pop(list, 0))

/**
 * @brief Pop the passed node, checking and updating list
 * @returns list's internal data type. ex: intList -> int
 */
#define LIST_POPNODE(list, node) (*(LIST_DTYPE(list)*)list_pop_node(list,node))


/**
 * @brief Get element at the given index.
 * if negative, search in reverse
 */
#define LIST_AT(list, index) (*(LIST_DTYPE(list)*)list_at(list, index))

/** 
 * @brief for wrapper for void*
 * 
 * @param cursor: variable name to use within the scope (access data: cursor->data)
 * 
 * @note use cursor->data to list's data at current iteration
 */
#define LIST_FOR_EACH(cursor, list)\
    for (__auto_type cursor = list->head;\
        cursor != NULL;\
        cursor = cursor->next)


/**
 * @brief Same as list_push() but item is passed by value
 */
#define LIST_PUSH(list, index, item) list_push(list, index, ((LIST_DTYPE(list)[]){item}))

/** 
 * @brief Create a list and push values if passed.
 * You must call list_delete() to free alocated memory
 * 
 * @param type: any defined type. ex: int, float, etc...
 * @param __VA_ARGS__: values to initialize list
 */
#define LIST_CREATE(type, ...) ({\
    typeof(type) _args[] = {__VA_ARGS__};\
    (type##List*)list_create(sizeof(type), sizeof(_args)/sizeof(*_args), _args);\
})


// ===== STRUCTURES ===== //

// data keeper
struct list_node {
    struct list_node* next;
    struct list_node* back;
    char data[];
};

// Generic List (macros do not work)
typedef struct list {
    size_t length;
    struct list_node* head;
    struct list_node* tail;
    struct {
        struct list_node* pop;
        const size_t dsize;
    } internal;
} List;

// ===== FUNCTIONS ===== //

/** 
 * @brief Create a list.
 * see LIST_CREATE() macro
 * 
 * @param dsize: size of each element in bytes
 * @param initial_size: initial size of the list. (0 is valid)
 * @param initial_values: pointer to data that will be pushed first. (0 is valid)
 */
void* list_create(size_t dsize, size_t initial_size, void *initial_values);

/**
 * @brief Push `num_elements` in `data` to list's end.
 * 
 * @param list: List, intList, floatList, etc...
 * @param num_elements: number of elements in data
 * @param data: array with values to be pushed (values will be copied)
 * 
 * @note see LIST_PUSHBACK(), it may be simpler to use
 */
void list_pushback(void* list, size_t num_elements, void *data);

/**
 * @brief Push `num_elements` in `data` to list's begin.
 * 
 * @param list: List, intList, floatList, etc...
 * @param num_elements: number of elements in data
 * @param data: array with values to be pushed (values will be copied)
 * 
 * @note see LIST_PUSHFRONT(), it may be simpler to use
 */
void list_pushfront(void* list, size_t num_elements, void *data);

/** 
 * @brief Push item in the index especified.
 * 
 * @param list: List, intList, floatList, etc...
 * @param index: position in list. if negative, search in reverse
 * @param item: reference to data, values will be copied
 */
void list_push(void* _list, int index, void * item);

/**
 * @brief Pop the element at index
 * 
 * @param list: List, intList, floatList, etc...
 * @param index: position in list. if negative, search in reverse
 * 
 * @returns: reference to value in deleted node
 * 
 * @note LIST_POP() return value instead of reference
 */
void* list_pop(void* list, int index);

/**
 * @brief Find a given index of list and return the data pointer
 * 
 * @param list: List, intList, floatList, etc...
 * @param index: position in list. if negative, search in reverse
 * 
 * @returns: reference to value
 * 
 * @note LIST_AT() return value instead of reference
 */
void* list_at(void* list, int index);

/**
 * @brief Receive some node from list and remove it properly
 * 
 * @param list: List, intList, floatList, etc...
 * @param node: any "struct list_node" or cursor from LIST_FOR_EACH() wrapper
 * 
 * @returns: reference to value in deleted node
 * 
 * @note: LIST_POP_NODE() return value instead of reference
 */
void* list_pop_node(void* list, void* node);

// Resize a list allocating new memory,
void list_resize(void* list, unsigned int new_size);

/// @brief makes a copy of list
void* list_copy(void* list);

/// @brief Create a sub list using the passed interval [begin, end)
void* list_slice(void* list, unsigned int begin, unsigned int end);

/// @brief Deletes all nodes and clear the list
void list_clear(void* list);

/// @brief Copy values to a array. (be sure to have suficient space in the array)
void list_to_array(void* list, void* result);

/// @brief free allocated memory
void list_delete(void* list);

/// @brief check equality of two lists
bool list_equals(void* a, void* b);

// Defining basic data lists
LIST_TYPEDEF(int);
LIST_TYPEDEF(float);