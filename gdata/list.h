/* 
 * Generic double linked List Library v1.5
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 * Define LIST_DEBUG to receive error messages.
 */

#pragma once
#include <stdlib.h>

// #define LIST_DEBUG

struct list_node {
    struct list_node *next;
    struct list_node *back;
    char data[];
};

typedef struct list {
    struct list_node* head;
    struct list_node* tail;
    size_t size;
    struct {
        struct list_node* iterator;
        struct list_node* pop;
        const size_t data_size;
    } internal;
} *List;


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

/* 
 * ### Create a list
 * 
 * if your data_size can be determined by sizeof() or not dealing with arrays,
 * list_create() may be simpler to use
 * 
 * Args:
 * * data_size: data size in bytes (all elements will allocate this size)
 * * initial_size: initial size of the list
 * * initial_values: pointer to data that will be pushed first
 * 
 * Usage:
 *  for Strings
 *   _list_create(32, 0, 0) -> list of char[32]
 *   _list_create(32, 2, (char[2][32]){"Hello", "World"}) -> ["Hello", "World"]
 * 
 *  for Arrays
 *   _list_create(10 * sizeof(int), 0, 0)  -> empty list of int[10]
 *   _list_create(5 * sizeof(float), 0, 0) -> empty list of float[5]
 */
List _list_create(size_t data_size, size_t initial_size, void * initial_values);

// =========================== PUBLIC FUNCTIONS =========================== //

/* 
 * ### Push value in list's end.
 * 
 * for variables, pass reference.
 * ex: `list_push_back(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `list_push_back(listName, (type[]){value})`
 */
void list_push_back(List list, void * item);

/* 
 * ### Push value in list's begin.
 * 
 * for variables, pass reference.
 * ex: `list_push_front(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `list_push_front(listName, (type[]){value})`
 */
void list_push_front(List list, void * item);

/* 
 * ### Push value in the index especified
 * if `index` is negative, searchs in reverse. 
 * 
 * for variables, pass reference.
 * ex: `list_push(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `list_push(listName, (type[]){value})`
 */
void list_push(List list, int index, void * item);

/*
 * ### Retrieve the in the index specified
 * if `index` is negative, searchs in reverse. 
 * cast to any type with `*(type*)`
 */
void *list_pop(List list, int index);

/* 
 * ### Delete the last node and return it's value.
 * cast to any type with `*(type*)`
 */
void * list_pop_back(List list);

/* 
 * ### Delete the first node and return it's value.
 * cast to any type with `*(type*)`
 */
void * list_pop_front(List list);

// ### Resize a list allocating new memory,
void list_resize(List list, unsigned int new_size);

/* 
 * ### Find a given index of list and return the data pointer
 * if index is negative, searchs in reverse. 
 * -1 = last, 0 = first
 * cast to any type with `*(type*)`
 */
void * list_at(List list, int index);

/* 
 * ### return one data per time from the list, then resets
 * you must expect `NULL` as the end
 * 
 * Usage: `for (type *data; data = list_for_each(list);)`
 */
void * list_for_each(List list);


// ### Copy all content of src list
List list_copy(List src);

// ### Create a sub list using the passed interval
List list_sublist(List list, unsigned int begin, unsigned int end);

// ### Remove the element of the given index
void list_remove(List list, int index);

/* 
 * ### Remove the current element in a list_for_each()
 * Removes at iterator position 
 */
void list_remove_iter(List list);

/* 
 * ### Deletes all nodes and clear the list
 */
void list_clear(List list);

// # Array related functions

/* 
 * ### Concatenate a Array on list's tail
 * 
 * ex: `list_push_array(list, 4, (double[]){1.0, 0.2, 3.4, 5.3});`
 * ex: `list_push_array(list, 6, array);`
 */
List list_push_array(List list, size_t num_elements, void * array);

/* 
 * ### Copy values to a array
 * be sure to have suficient space in the array
 */
void list_to_array(List list, void* array);

/* 
 * ### Create a list and push values if passed
 * use _list_create() for strings or advanced creation of list
 * 
 * Args:
 * * type: some data type. ex: int, float, YourType...
 * * initial_values: initialize with these values
 * 
 * Usage:
 * * Ex: `list_create(int)`
 * * Ex: `list_create(double, 2.7, 3.2, 0.5, 0.1, 0.9)`
 * 
 * Obsevations:
 * * You must call `list_delete()` to free alocated memory
 */
#define list_create(type, initial_values...) ({\
    type a[] = {initial_values};\
    _list_create(sizeof(type), sizeof(a)/sizeof(type), a);\
})

/* 
 * ### Destructor 
 * Deletes a Created List
 */
void list_delete(List list);