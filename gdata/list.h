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
    struct list_node *head;
    struct list_node *tail;
    void (*valueDestructor)(void* value);
    size_t size;

    struct list_node *iterator;
    const size_t data_size;
} *List;


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

List _list_create(size_t data_size, size_t size, void * values);

// =========================== PUBLIC FUNCTIONS =========================== //

/* 
 * ### Push value in list's end.
 * 
 * for variables, pass reference.
 * ex: `List_pushBack(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `List_pushBack(listName, (type[]){value})`
 */
void List_pushBack(List list, void * item);

/* 
 * ### Push value in list's begin.
 * 
 * for variables, pass reference.
 * ex: `List_pushFront(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `List_pushFront(listName, (type[]){value})`
 */
void List_pushFront(List list, void * item);

/* 
 * ### Push value in the index especified
 * if `index` is negative, searchs in reverse. 
 * 
 * for variables, pass reference.
 * ex: `List_push(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `List_push(listName, (type[]){value})`
 */
void List_push(List list, int index, void * item);

/*
 * ### Retrieve the in the index specified
 * if `index` is negative, searchs in reverse. 
 * cast to any type with `*(type*)`
 */
void *List_pop(List list, int index);

/* 
 * ### Delete the last node and return it's value.
 * cast to any type with `*(type*)`
 */
void * List_popBack(List list);

/* 
 * ### Delete the first node and return it's value.
 * cast to any type with `*(type*)`
 */
void * List_popFront(List list);

/* 
 * ### Resize a list allocating new memory,
 */
void List_resize(List list, unsigned int new_size);

/* 
 * ### Find a given index of list and return the data pointer
 * if index is negative, searchs in reverse. 
 * -1 = last, 0 = first
 * cast to any type with `*(type*)`
 */
void * List_at(List list, int index);

/* 
 * ### return one data per time from the list, then resets
 * you must expect `NULL` as the end
 * 
 * Usage: `for (type *data; data = List_forEach(list);)`
 */
void * List_forEach(List list);

/* 
 * ### Copy all content of src list
 */
List List_copy(List src);

/* 
 * ### Remove the element of the given index
 */
void List_remove(List list, int index);

/* 
 * ### Remove the current element in a List_forEach()
 * Removes at iterator position 
 */
void List_removeIterator(List list);

/* 
 * ### Deletes all nodes and clear the list
 */
void List_clear(List list);

// # Array related functions

/* 
 * ### Concatenate a Array on list's tail
 * 
 * ex: `List_pushArray(list, 4, (double[]){1.0, 0.2, 3.4, 5.3});`
 * ex: `List_pushArray(list, 6, array);`
 */
List List_pushArray(List list, size_t num_elements, void * array);

/* 
 * ### convert to a array
 * you must cast to any type with `(type*)`
 * memory is alocated, you must free after using!
 */
void * List_toArray(List list);


/* 
 * ### Create a list based on a existent array
 */
#define ListCreate_fromArray(array, array_size)\
    _list_create(sizeof(*array), array_size, array)

/* 
 * ### Create a list based on a array literal
 * Args:
 * * type: type of your list. ex: int, float, YourType...
 * * array: literal in `{...}`. use `ListCreate_fromArray()` for declared arrays
 * 
 * Obsevations:
 * * You must call `ListDelete()` to free alocated memory
 * * `valueDestructor` will be called when removing, cleaning or deleting the list,
 *   Set if you need to free allocated memory holded by your data.
 *   Ex: `myList->valueDestructor = MyDestructor;`
 * 
 * Usage:
 * * Ex: `ListCreate(int, {})`
 * * Ex: `ListCreate(double, {2.7, 3.2, 0.5, 0.1, 0.9})`
 */
#define ListCreate(type, array...) ({\
    type a[] = array;\
    _list_create(sizeof(type), sizeof(a)/sizeof(type), a);\
})

/* 
 * ### Destructor 
 * Deletes a Created List
 */
void ListDelete(List *list);