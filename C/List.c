/* 
 * Generic double linked List Library v1.4
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */

#include "List.h"
#include <memory.h>

#ifdef LIST_DEBUG
# include <stdio.h>
#endif

// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //
/* 
 * Allocate a list_node and return the pointer
 * obs: size in bytes
 */
struct list_node *_list_new_node(size_t size) {
    struct list_node *ptr = calloc(1,sizeof(struct list_node));
    if (ptr)
        ptr->data = calloc(1,size);
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Failed to create node");
#endif
    return ptr;
}

/* 
 * Free the given node conecting neighbors to each other
 * _node->data is **not** freed_
 */
void _list_del_node(struct list_node *n) {
    if (n) {
        if (n->back) n->back->next = n->next;
        if (n->next) n->next->back = n->back;
        free(n);
    }
}

/* 
 * Find the linked node by the given index.
 */
struct list_node * _list_node_find(struct list_node * node, int index) {
    if (index == 0 || index == -1)
        return node;

    _Bool reverse = index < 0;
    struct list_node *next = reverse ? node->back : node->next;
    return _list_node_find(next, index + (reverse ? 1 : -1));
}

/* 
 * List constrained index finder 
 * if index is negative, searchs in reverse. 
 * return: the node
 */
struct list_node * _list_node_at(List list, int index) {
    if (index >= 0)
        return _list_node_find(list->head, index);
    else
        return _list_node_find(list->tail, index);
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: Index out of the list!");
#endif
    return NULL;
}

void * _list_at(List list, int index) {
    return _list_node_at(list, index)->data;
}

// ### Constructor
List _list_create(size_t data_size, size_t size, void * values) {
    List list = calloc(1,sizeof(struct list));
    *(size_t*)&list->data_size = data_size;
    if (values)
        List_pushArray(list, size, values);
    return list;
}


// =========================== PUBLIC FUNCTIONS =========================== //

// Push value in list's end.
void List_pushBack(List list, void * item) {
    struct list_node *new_node = _list_new_node(list->data_size);

    if (list->size > 0) {
        list->tail->next = new_node;
        new_node->back = list->tail;
        list->tail = new_node;
    }
    else list->head = list->tail = list->iterator = new_node;

    memcpy(new_node->data, item, list->data_size);
    list->size++;
}

// Push value in list's begin.
void List_pushFront(List list, void * item) {
    struct list_node *new_node = _list_new_node(list->data_size);

    if (list->size > 0) {
        list->head->back = new_node;
        new_node->next = list->head;
        list->iterator = list->head = new_node;
    }
    else list->head = list->tail = list->iterator = new_node;

    memcpy(new_node->data, item, list->data_size);
    list->size++;
}

//Push value in the index especified
void List_push(List list, int index, void * item) {

    struct list_node *old_node = NULL;

    if (list->size == 0)
        return List_pushFront(list, item);
    old_node = _list_node_at(list, index);

    if (old_node) {
        struct list_node *new_node = _list_new_node(list->data_size);
        if (new_node) {
            // Goes before old item
            if (index >= 0) {
                new_node->next = old_node;
                new_node->back = old_node->back;
                if (old_node == list->head)
                    list->head = list->iterator = new_node;
                else
                    old_node->back->next = new_node;
                old_node->back = new_node;

            // Goes after old item
            } else {
                new_node->back = old_node;
                new_node->next = old_node->next;
                if (old_node == list->tail)
                    list->tail = new_node;
                else
                    old_node->next->back = new_node;
                old_node->next = new_node;
            }
            memcpy(new_node->data, item, list->data_size);
            list->size++;
        }
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Index not found!");
#endif

}

// Retrieve the in the index specified
void *List_pop(List list, int index) {
    struct list_node * n = NULL;
    if (list->size > 0) {
        if (index == 0) {
            n = list->head;
            list->iterator = list->head = n->next;
        } 
        else if (index == -1) {
            n = list->tail;
            list->tail = n->back;
        }
        else n = _list_node_at(list, index);

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

// Delete the last node and return it's value.
void * List_popBack(List list) {
    return List_pop(list, -1);
}

// Delete the first node and return it's value.
void * List_popFront(List list) {
    return List_pop(list, 0);
}

// Resize a list allocating new memory,
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
            List_remove(list, -1);
    }
}

// Return one data per time from the list, then resets
void * List_forEach(List list) {
    if (list->iterator) {
        void * data = list->iterator->data;
        list->iterator = list->iterator->next;
        return data;
    }
    list->iterator = list->head;
    return NULL;
}

// Returns a copy of `src`
List List_copy(List src) {
    List dst = _list_create(src->data_size, 0, 0);
    for (void *data; data = List_forEach(src);)
        List_pushBack(dst, data);
    return dst;
}

void List_remove(List list, int index) {
    if (list->valueDestructor)
        list->valueDestructor(List_pop(list, index));
    else
        free(List_pop(list, index));
}

void List_clear(List list) {
    while (list->size > 0)
        List_remove(list, -1);
    list->iterator = list->head = list->tail =NULL;
}


// # Array related functions

List List_pushArray(List list, size_t num_elements, void * array) {
    for (size_t i = 0; i < num_elements; i++) {
        // jumping `i` bytes
        void * array_element = array + i*list->data_size;
        List_pushBack(list, array_element);
    }
    return list;
}

// convert to a array
void * List_toArray(List list) {
    void * array = calloc(list->size, list->data_size);
    struct list_node *n = list->head;
    for (size_t i = 0; i < list->size; i++) {
        void * array_element = (char*)array + i*list->data_size;
        memcpy(array_element, n->data, list->data_size);
        n = n->next;
    }
    return array;
}

void ListDelete(List *list) {
    List_clear(*list);
    free(*list);
}