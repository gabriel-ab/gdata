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

/* 
 * ### Resize a list allocating new memory,
 */
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

/* 
 * ### Copy all content of src list
 */
List list_copy(List src);

/* 
 * ### Remove the element of the given index
 */
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
 * ### convert to a array
 * you must cast to any type with `(type*)`
 * memory is alocated, you must free after using!
 */
void * list_to_array(List list);

/* 
 * ### Create a list and push values if passed
 * pass values separated by comma (,)
 * Args:
 * * type: type of your list. ex: int, float, YourType...
 * 
 * Usage:
 * * Ex: `list_create(int)`
 * * Ex: `list_create(double, 2.7, 3.2, 0.5, 0.1, 0.9)`
 * 
 * Obsevations:
 * * You must call `list_delete()` to free alocated memory
 * 
 * * `valueDestructor` will be called when removing, cleaning or 
 *   deleting the list, set if you need to free allocated memory 
 *   holded by your data.
 *   Ex: `myList->valueDestructor = MyDestructor;`
 */
#define list_create(type, values...) ({\
    type a[] = {values};\
    _list_create(sizeof(type), sizeof(a)/sizeof(type), a);\
})

/* 
 * ### Destructor 
 * Deletes a Created List
 */
void list_delete(List *list);