/* ====  Made by Gabriel-AB  ====
 * Generic double linked List Library v1.3
 * 
 * https://github.com/Gabriel-AB
 * 
 * Define LIST_DEBUG before inclusion to receive error messages.
 */

#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <memory.h>

#ifdef LIST_DEBUG
# include <stdio.h>
#endif

struct node {
    struct node *next;
    struct node *back;
    void *data;
};

typedef struct list {
    struct node *head;
    struct node *tail;
    struct node *iterator;
    size_t size;
    const size_t data_size;
    void (*valueDestructor)(void *value);
} *List;


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
/* 
 * Allocate a node and return the pointer
 * obs: size in bytes
 */
struct node *_list_new_node(size_t size) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if (ptr) {
        ptr->back = NULL;
        ptr->next = NULL;
        ptr->data = calloc(1,size);
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Failed to create node");
#endif
    return ptr;
}

/* 
 * Free the given node conecting neighbors to each other
 * _node->data is **not** freed_
 * 
 */
void _list_del_node(struct node *n) {
    if (n) {
        if (n->back) n->back->next = n->next;
        if (n->next) n->next->back = n->back;
        free(n);
    }
}

/* 
 * Find the linked node by the given index.
 */
struct node * _list_node_find(struct node * node, int index) {
    if (index == 0 || index == -1)
        return node;

    _Bool reverse = index < 0;
    struct node *next = reverse ? node->back : node->next;
    return _list_node_find(next, index + (reverse ? 1 : -1));
}

/* 
 * List constrained index finder 
 * if index is negative, searchs in reverse. 
 * return: the node
 */
struct node * _list_at(List list, int index) {
    if (abs(index) < list->size) {
        if (index >= 0)
            return _list_node_find(list->head, index);
        else
            return _list_node_find(list->tail, index);
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Index out of the list!");
#endif
    return NULL;
}

// ### Constructor
List _list_create(size_t data_size) {
    List list = (List)malloc(sizeof(struct list));
    (*(size_t*)&list->data_size) = data_size;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


// =========================== PUBLIC FUNCTIONS =========================== //

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
void List_push(List list, int index, void * item) {
    struct node *old_node = NULL;
    struct node *new_node = _list_new_node(list->data_size);

    if (new_node) {
        if (list->size > 0) {
            if (index == 0) {
                old_node = list->head;
                list->iterator = list->head = new_node;
            }
            else if (index == -1) {
                old_node = list->tail;
                list->tail = new_node;
            }
            else old_node = _list_at(list, index);
        } else {
            list->head = list->tail = list->iterator = new_node;
        }

        if (old_node) {
            if (index >= 0) { // Goes before old item
                new_node->next = old_node;
                new_node->back = old_node->back;
                if(old_node->back)
                    old_node->back->next = new_node;
                old_node->back = new_node;
            } else { // Goes after old item
                new_node->back = old_node;
                new_node->next = old_node->next;
                if(old_node->next)
                    old_node->next->back = new_node;
                old_node->next = new_node;
            }
        }
        memcpy(new_node->data, item, list->data_size);
        list->size++;
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Failed to push");
#endif
}


/* 
 * ### Push value in list's end.
 * 
 * for variables, pass reference.
 * ex: `List_pushBack(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `List_pushBack(listName, (type[]){value})`
 */
void List_pushBack(List list, void * item) {
    List_push(list, -1, item);
}

/* 
 * ### Push value in list's begin.
 * 
 * for variables, pass reference.
 * ex: `List_pushFront(listName, &myItem)`
 * 
 * for literals use `(type[]){value}`
 * ex: `List_pushFront(listName, (type[]){value})`
 */
void List_pushFront(List list, void * item) {
    List_push(list, 0, item);
}

/*
 * ### Retrieve the in the index specified
 * if `index` is negative, searchs in reverse. 
 * The data is alocated, you must free later
 */
void *List_pop(List list, int index) {
    struct node * n = NULL;
    if (list->size > 0) {
        if (index == 0) {
            n = list->head;
            list->iterator = list->head = n->next;
        } 
        else if (index == -1) {
            n = list->tail;
            list->tail = n->back;
        }
        else n = _list_at(list, index);

        if (n) {
            list->size--;
            void * data = n->data;
            _list_del_node(n);
            return data;
        }
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: List_pop(): List is empty");
#endif
    return NULL;
}

/* 
 * ### Delete the last node and return it's value.
 * you must cast to your type with `*(type*)`
 */
void * List_popBack(List list) {
    return List_pop(list, -1);
}

/* 
 * ### Delete the first node and return it's value.
 * you must cast to your type with `*(type*)`
 */
void * List_popFront(List list) {
    return List_pop(list, 0);
}

/* 
 * ### Resize a list allocating new memory,
 */
void List_resize(List list, unsigned int new_size) {
    if (list->size < new_size) {
        int count = new_size -list->size;
        for (int i = 0; i < count; i++) {
            void * item = calloc(1,list->data_size);
            List_pushBack(list, item);
        }
            
    } else {
        int count = list->size -new_size;
        for (int i = 0; i < count; i++)
            free(List_popBack(list));
    }
}

/* 
 * ### Find a given index of list and return the data pointer
 * if index is negative, searchs in reverse. 
 * ex: `List_at(yourlist, -1)` = last element
 */
void * List_at(List list, int index) {
    return _list_at(list, index)->data;
}

/* 
 * ### return one data per time from the list, then resets
 * you must expect `NULL` as the end
 * 
 * using *for*: 
 * - `for (type *data; (data = List_forEach(list));) {}`
 * using *while*: 
 * - `type* info; while(info = List_forEach(lista)) {}`
 */
void * List_forEach(List list) {
    if (list->iterator) {
        void * data = list->iterator->data;
        list->iterator = list->iterator->next;
        return data;
    }
    list->iterator = list->head;
    return NULL;
}

/* 
 * ### Copy all content of src list
 */
List List_copy(List src) {
    List dst = _list_create(src->data_size);
    void *data;
    while ( (data = List_forEach(src)) )
        List_pushBack(dst, data);

    return dst;
}

/* 
 * ### Remove the element of the given index
 * 
 */
void List_remove(List list, int index) {
    if (list->valueDestructor)
        list->valueDestructor(List_pop(list, index));
    else
        free(List_pop(list, index));
}

/* 
 * ### Deletes all nodes and clear the list
 */
void List_clear(List list) {
    while (list->size > 0)
        List_remove(list, -1);
    list->iterator = list->head = list->tail =NULL;
}

/* 
 * ### Create a Empty List holding the type size
 * _Argument_
 * * type: ex: int, float, YourType...
 * 
 * _Obsevations_
 * * You must call `ListDelete()` to free alocated memory
 * * This macro is a Syntatic sugar to `_list_create()`
 * * set `valueDestructor` if you need to free allocated memory holded by your data
 *   ex: `myList->valueDestructor = MyDestructor;`
 */
#define ListCreate(type) _list_create(sizeof(type))

/* 
 * ### Destructor 
 * Deletes a Created List
 */
void ListDelete(List list) {
    List_clear(list);
    free(list);
}

// # Array related functions

/* 
 * ### Concatenate a Array on list's tail
 */
void List_pushArray(List list, size_t num_elements, void * array) {
    for (size_t i = 0; i < num_elements; i++) {
        // jumping `i` bytes
        void * array_element = array + i*list->data_size;
        List_pushBack(list, array_element);
    }
}

/* 
 * ### Create a list based on a array
 */
List _list_fromArray(size_t data_size, size_t num_elements, void * array) {
    List list = _list_create(data_size);
    List_pushArray(list, num_elements, array);
    return list;
}
/* 
 * ### Create a list based on a named array
 * Args:
 * * type: type of your list. ex: int, float, YourType...
 * * num_elements: positive integer
 * * array: variable or literal.
 * 
 * Ex: `List_fromArray(double, 5, myArray)`
 * Ex: `List_fromArray(double, 5, (double[]){2.7, 3.2, 0.5, 0.1, 0.9})`
 */
#define List_fromArray(type, num_elements, array...)\
    _list_fromArray(sizeof(type), num_elements, array)


/* 
 * ### convert to a array
 * you must cast to your type with `(type*)`
 * memory is alocated, you must free after using!
 */
void * List_toArray(List list) {
    void * array = malloc(list->size * list->data_size);
    struct node *n = list->head;
    for (size_t i = 0; i < list->size; i++) {
        void * array_element = (char*)array + i*list->data_size;
        memcpy(array_element, n->data, list->data_size);
        n = n->next;
    }
    return array;
}

#endif // LIST_H