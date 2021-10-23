#include "dict.h"

#define MAX_KEY_SIZE 32

struct dict_pair {
    struct dict_pair* next;
    void(*del)(void*);
    char key[MAX_KEY_SIZE];
    void* value;
};

struct dict {
    struct dict_pair** children;
    size_t length;
    size_t max_size;
};

// djb2 hash function
static size_t hash(unsigned char *str)
{
    size_t hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

Dict dict_create(size_t size) {
    Dict result = malloc(sizeof(*result));
    result->children = calloc(size, sizeof(struct dict_pair*));
    result->max_size = size;
    result->length = 0;
    return result;
}

void dict_delete(Dict dict) {
    dict_clear(dict);
    free(dict->children);
    free(dict);
}

size_t dict_size(Dict dict) {
    return dict->length;
}

void dict_set(Dict dict, const char* key, void* value, void(*destructor)(void*)) {
    size_t index = hash((unsigned char*)key) % dict->max_size;
    struct dict_pair* node = dict->children[index];
    bool empty_list = !node;

    // Check the linked list for key, if it's not, return the last node to append a new one
    while(node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            node->del = destructor;
            return;
        }
        if (!node->next) break;
        node = node->next;
    }

    struct dict_pair* new_node = malloc(sizeof(struct dict_pair));
    if (new_node) {
        strncpy(new_node->key, key, MAX_KEY_SIZE);
        new_node->value = value;
        new_node->del = destructor;
        dict->length++;
        if (empty_list)
            dict->children[index] = new_node;
        else
            node->next = new_node;
    }
}

void dict_setobj(Dict dict, const char* key, void* value) {
    dict_set(dict, key, value, free);
}

void dict_setref(Dict dict, const char* key, void* value) {
    dict_set(dict, key, value, NULL);
}

void* dict_get(Dict dict, const char* key) {
    size_t index = hash((unsigned char*)key) % dict->max_size;
    struct dict_pair* node = dict->children[index];
    while(node) {
        if (strcmp(node->key, key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL;
}

void dict_clear(Dict dict) {
    for (size_t i = 0; i < dict->max_size; i++) {
        struct dict_pair* curr = dict->children[i], *next;
        while (curr) {
            next = curr->next;
            if (curr->del) curr->del(curr->value);
            free(curr);
            curr = next;
        }
    }
}