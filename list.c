/* 
 * Generic double linked List Library v1.4
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 * 
 */

#include "list.h"
#include <string.h>

#ifdef LIST_DEBUG
# include <stdio.h>
#endif


// ====================== LIBRARY INTERNAL FUNCTIONS ====================== //

/* 
 * Allocate a list_node and return the pointer
 * obs: size in bytes
 */
struct list_node *_list_new_node(size_t size) {
    struct list_node *ptr = calloc(1,sizeof(struct list_node) + size);
#ifdef LIST_DEBUG
    if (!ptr) perror("LIST_DEBUG: Failed to create node");
#endif
    return ptr;
}

/* 
 * Free the given node conecting neighbors to each other
 * and return the allocated data pointer
 */
void _list_detach_node(struct list_node *n) {
    if (n->back) n->back->next = n->next;
    if (n->next) n->next->back = n->back;
}

/* 
 * Find the linked node by the given index.
 */
struct list_node * _list_find(struct list_node * node, int index) {
    if (index < 0) {
        while (++index && node)
            node = node->back;
    } else {
        while (index-- && node)
            node = node->next;
    }
    return node;
}

/* 
 * List constrained index finder 
 * if index is negative, searchs in reverse. 
 * return: the node
 */
struct list_node * _list_node_at(List list, int index) {
#ifdef LIST_DEBUG
    if (list->size < (size_t)(index < 0 ? -index-1 : index))
        perror("LIST_DEBUG: Index out of the list!");
#endif
    return _list_find((index < 0) ? list->tail : list->head, index);
}

void * list_at(List list, int index) {
    return _list_node_at(list, index)->data;
}

// ### Constructor
List _list_create(size_t data_size, size_t initial_size, void * initial_values) {
    List list = calloc(1,sizeof(struct list));
    *(size_t*)&list->internal.data_size = data_size;
    if (initial_values)
        list_push_array(list, initial_size, initial_values);
    else if (initial_size)
        list_resize(list, initial_size);
    return list;
}


// =========================== PUBLIC FUNCTIONS =========================== //

// Push value in list's end.
void list_push_back(List list, void * item) {
    struct list_node *new_node = _list_new_node(list->internal.data_size);

    if (item) memcpy(new_node->data, item, list->internal.data_size);

    if (list->size++ > 0) {
        list->tail->next = new_node;
        new_node->back = list->tail;
        list->tail = new_node;
    }
    else list->head = list->tail = list->internal.iterator = new_node;
}

// Push value in list's begin.
void list_push_front(List list, void * item) {
    struct list_node *new_node = _list_new_node(list->internal.data_size);

    if (item) memcpy(new_node->data, item, list->internal.data_size);

    if (list->size++ > 0) {
        list->head->back = new_node;
        new_node->next = list->head;
        list->internal.iterator = list->head = new_node;
    }
    else list->head = list->tail = list->internal.iterator = new_node;
}

//Push value in the index especified
void list_push(List list, int index, void * item) {
    struct list_node *old_node = _list_node_at(list, index);
    if (old_node) {
        struct list_node *new_node = _list_new_node(list->internal.data_size);
        if (new_node) {
            // Goes before old item
            if (index >= 0) {
                new_node->next = old_node;
                new_node->back = old_node->back;
                if (old_node == list->head)
                    list->head = list->internal.iterator = new_node;
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
        }
        if (item) memcpy(new_node->data, item, list->internal.data_size);
        list->size++;
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Index not found!");
#endif

}

// Retrieve the in the index specified
void *list_pop(List list, int index) {
    struct list_node * n = _list_node_at(list, index);
    if (n) {
        if (n == list->tail)
            list->tail = n->back;
        if (n == list->head)
            list->internal.iterator = list->head = n->next;
        list->size--;

        if (list->internal.pop)
            free(list->internal.pop);
        _list_detach_node(n);
        list->internal.pop = n;

        return n->data;
    }
    return NULL;
}

// Delete the last node and return it's value.
void * list_pop_back(List list) {
    return list_pop(list, -1);
}

// Delete the first node and return it's value.
void * list_pop_front(List list) {
    return list_pop(list, 0);
}

// Resize a list allocating new memory,
void list_resize(List list, unsigned int new_size) {
    if (list->size < new_size) {
        int count = new_size -list->size;
        for (int i = 0; i < count; i++) {
            void * item = calloc(1,list->internal.data_size);
            list_push_back(list, item);
        }
            
    } else {
        int count = list->size -new_size;
        for (int i = 0; i < count; i++)
            list_remove(list, -1);
    }
}

// Return one data per time from the list, then resets
void * list_for_each(List list) {
    if (list->internal.iterator) {
        void * data = list->internal.iterator->data;
        list->internal.iterator = list->internal.iterator->next;
        return data;
    }
    list->internal.iterator = list->head;
    return NULL;
}

// Returns a copy of `src`
List list_copy(List src) {
    List dst = _list_create(src->internal.data_size, 0, 0);
    for (void *data; (data = list_for_each(src));)
        list_push_back(dst, data);
    return dst;
}

void list_remove(List list, int index) {
    list_pop(list, index);
}

void list_remove_iter(List list) {
    if (list->internal.iterator == NULL) {
        list_remove(list, -1);
        return;
    }

    struct list_node * current = list->internal.iterator->back;
    if (current) {
        if (current == list->head) {
            list_remove(list, 0);
            return;
        }
        
        _list_detach_node(current);
        free(current);
        list->size--;
    }
}

void list_clear(List list) {
    while (list->size > 0)
        list_remove(list, -1);
    free(list->internal.pop);
    list->internal.pop = NULL;
}


// # Array related functions

List list_push_array(List list, size_t num_elements, void * array) {
    for (size_t i = 0; i < num_elements; i++) {
        void * array_element = (char*)array + i*list->internal.data_size;
        list_push_back(list, array_element);
    }
    return list;
}

// convert to a array
void list_to_array(List list, void* array) {
    struct list_node *n = list->head;
    for (size_t i = 0; i < list->size; i++) {
        void * array_element = (char*)array + i*list->internal.data_size;
        memcpy(array_element, n->data, list->internal.data_size);
        n = n->next;
    }
}

void list_delete(List list) {
    list_clear(list);
    free(list);
}

List list_sublist(List list, unsigned int begin, unsigned int end) {
    List result = list_copy(list);
    end = list->size - end;
    while (begin--)
        list_pop_front(result);
    while (end--)
        list_pop_back(result);
    return result;
}