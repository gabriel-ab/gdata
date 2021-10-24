#include "dict.h"

#define DICT_MAX_KEY_SIZE 64

struct dict_pair {
    struct dict_pair* next;
    void(*del)(void*);
    char key[DICT_MAX_KEY_SIZE];
    void* value;
};

struct dict {
    struct dict_pair** hashtable;
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

Dict dict_create(size_t hashtable_size) {
    Dict result = malloc(sizeof(*result));
    result->hashtable = calloc(hashtable_size, sizeof(struct dict_pair*));
    result->max_size = hashtable_size;
    result->length = 0;
    return result;
}

void dict_delete(Dict dict) {
    dict_clear(dict);
    free(dict->hashtable);
    free(dict);
}

size_t dict_size(Dict dict) {
    return dict->length;
}

void dict_set(Dict dict, const char* key, void* value, void(*destructor)(void*)) {
    size_t index = hash((unsigned char*)key) % dict->max_size;
    struct dict_pair* node = dict->hashtable[index];
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
        strncpy(new_node->key, key, DICT_MAX_KEY_SIZE);
        new_node->value = value;
        new_node->del = destructor;
        dict->length++;
        if (empty_list)
            dict->hashtable[index] = new_node;
        else
            node->next = new_node;
        
        // Setting flag to update dict keys if they exist
        if (dict->hashtable[0] && dict->hashtable[0]->key[0] == '\0')
            dict->hashtable[0]->key[1] = '\1';
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
    struct dict_pair* node = dict->hashtable[index];
    while(node) {
        if (strcmp(node->key, key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL;
}

void dict_remove(Dict dict, const char* key) {
    size_t index = hash((unsigned char*)key) % dict->max_size;
    struct dict_pair* a = dict->hashtable[index], *b = a;
    while (a && strncmp(a->key, key, DICT_MAX_KEY_SIZE)) b = a;
    if (a) {
        b->next = a->next;
        if (a->del) a->del(a->value);
        free(a);
        dict->length--;
    }
}

void dict_clear(Dict dict) {
    for (size_t i = 0; i < dict->max_size; i++) {
        struct dict_pair* curr = dict->hashtable[i], *next;
        while (curr) {
            next = curr->next;
            if (curr->del) curr->del(curr->value);
            free(curr);
            curr = next;
        }
    }
}

const char ** dict_keys(Dict dict) {
    struct dict_pair* node = dict->hashtable[0], *search_node;

    if (node == NULL || node && node->key[0] != '\0') {
        struct dict_pair* new_node = malloc(sizeof(struct dict_pair));
        new_node->next = node;
        new_node->del = free;
        node = dict->hashtable[0] = new_node;
        node->key[1] = 1;
    }
    const char ** list = node->value;
    
    if (node->key[1] == 1) {
        list = realloc(list, sizeof(char*)*dict->length);

        size_t curr = 0, i = 0;
        search_node = dict->hashtable[i++]->next;

        // search the first index ignoring
        while (search_node) {
            list[curr++] = search_node->key;
            search_node = search_node->next;
        }

        while (i < dict->max_size) {
            search_node = dict->hashtable[i++];
            while (search_node) {
                list[curr++] = search_node->key;
                search_node = search_node->next;
            }
        }
        node->key[1] = '\0';
        node->value = list;
    }
    return list;
}