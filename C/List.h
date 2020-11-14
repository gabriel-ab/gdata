/* ====  Made by Gabriel-AB  ====
 * Generic double linked List Library 1.1
 * 
 * Define LIST_DEBUG before inclusion to receive error messages.
 */

#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <memory.h>

struct node {
    struct node *next;
    struct node *back;
    void *data;
};

typedef struct list {
    struct node *head;
    struct node *tail;
    size_t size;
    const size_t data_size;
    char type_name[32];
} *List;


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
 * Find the linked node by the given index.
 */
struct node * _list_node_find(struct node * node, int index) {
    if (index == 0 || index == -1)
        return node;

    _Bool reverse = index < 0;
    struct node *next = reverse ? node->back : node->next;
    return _list_node_find(next, index + (reverse ? 1 : -1));
}

// ============ PUBLIC FUNCTIONS ============ //

// ### CONSTRUCTORS
/* 
 * ### Create a Empty List holding the type size
 * Obs: you must call ListDelete() to free alocated memory
 */

List _list_create(size_t data_size, char type_name[32]) {
    List list = (List)malloc(sizeof(struct list));
    (*(size_t*)&list->data_size) = data_size;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    memcpy(list->type_name, type_name, 32);
    return list;
}
#define ListCreate(type) _list_create(sizeof(type), #type)

/* 
 * ### DESTRUCTOR 
 * Free all nodes from a Created List, and clean it.
 */
void ListDelete(List list) {
    _list_del_all_nodes(list->head);
    free(list);
}


/* 
 * ### Create a node in list's tail and atribute it's value.
 */
void List_pushBack(List list, void * item) {
    struct node *n = _list_new_node(list->data_size);
    if (n) {
        n->next = NULL;
        n->back = list->tail;
        if (list->tail)
            list->tail->next = n;
        else
            list->head = n;
        list->tail = n;
        list->size++;
        if (item) memcpy(n->data, item, list->data_size);
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Failed to create node at list's tail");
#endif
}

/* 
 * ### Create a node in list's head and atribute it's value.
 */
void List_pushFront(List list, void * item) {
    struct node *n = _list_new_node(list->data_size);
    if (n) {
        n->back = NULL;
        n->next = list->head;
        if (list->head)
            list->head->back = n;
        else
            list->tail = n;
        list->head = n;
        list->size++;
        if (item) memcpy(n->data, item, list->data_size);
    }
#ifdef LIST_DEBUG
    else perror("LIST_DEBUG: Failed to create node at list's head");
#endif
}

/* 
 * ### Delete the last node and return it's value.
 * you must cast to your type with `*(type*)`
 */
void * List_popBack(List list) {
    if (list->size > 0) {
        void * data = list->tail->data;
        if (list->size == 1) {
            _list_del_node(list->tail, 0);
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->tail = list->tail->back;
            _list_del_node(list->tail->next, 0);
            list->tail->next = NULL;
        }
        list->size--;
        return data;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: List is empty");
#endif
    return NULL;
}

/* 
 * ### Delete the first node and return it's value.
 * you must cast to your type with `*(type*)`
 */
void * List_popFront(List list) {
    if (list->size > 0) {
        void* result = list->head->data;
        if (list->size == 1) {
            _list_del_node(list->head, 0);
            list->head = NULL;
            list->tail = NULL;
        } else {
            list->head = list->head->next;
            _list_del_node(list->head->back, 0);
            list->head->back = NULL;
        }
        list->size--;
        return result;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: List is empty");
#endif
    return NULL;
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
    if (abs(index) < list->size) {
        if (index >= 0)
            return _list_node_find(list->head, index)->data;
        else
            return _list_node_find(list->tail, index)->data;
    }
#ifdef LIST_DEBUG
    perror("LIST_DEBUG: Index out of the list!");
#endif
    return NULL;
}

/* 
 * ### Map a function through the list
 * The function must take data by reference
 */
void List_forEach(List list, void (*function)(void*)) {
    struct node * node = list->head;
    do {
        function(node->data);
    } while(node = node->next);
}

/* 
 * ### Copy all content of src to dst
 * The list type must be the same.
 */
void List_copy(List dst, List src) {
    ListDelete(dst);
    dst = _list_create(src->data_size, "");
    memcpy(dst->type_name, src->type_name, 32);
    
    List_resize(dst, src->size);
    struct node *dst_node = dst->head;
    struct node *src_node = src->head;

    while (dst_node != NULL) {
        memcpy(dst_node->data, src_node->data, src->data_size);
        
        dst_node = dst_node->next;
        src_node = src_node->next;
    }
}

/* 
 * ### Deletes all nodes and clear the list
 */
void List_clear(List list) {
    _list_del_all_nodes(list->head);
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

/* 
 * ### Remove the element of the given index
 */
void List_remove(List list, int index) {
    _list_del_node(_list_node_find(list, index), 1);
    list->size--;
}

/*
 * ### Retrieve the in the index specified
 * The data is alocated, you must free later
 */
void *List_pop(List list, int index) {
    struct node * n = _list_node_find(list->head, index);
    void * data = n->data;
    _list_del_node(n, 0);
    list->size--;
    return data;
}


// # Array related functions

/* 
 * ### Concatenate a Array on list's tail
 */
void List_pushArray(List list, size_t num_elements, void * array) {
    for (size_t i = 0; i < num_elements; i++) {
        // jumping `i` bytes
        void * array_element = (char*)array + i*list->data_size;
        List_pushBack(list, array_element);
    }
}

/* 
 * ### Resizes the list to array size, then copy all values
 */
void List_fromArray(List list, size_t num_elements, void * array) {
    List_resize(list, num_elements);
    for (int i = 0; i < num_elements; i++) {
        void * array_element = (char*)array + i*list->data_size;
        memcpy(List_at(list,i), array_element, list->data_size);
    }
}

/* 
 * ### convert to a array
 * you must cast to your type with `(type*)
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

// # UTILS
/* ### Transforms a literal in a reference to itself
 * good to pass literals into the list
 */
#define List_litToRef(literal...) ({typeof(literal) res = literal; &res;})

#endif // LIST_H