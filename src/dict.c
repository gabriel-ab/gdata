#include "dict.h"

#define DICT_MAX_KEY_SIZE 64

struct dict_pair {
    struct dict_pair* next;
    void(*del)(void*);
    char key[DICT_MAX_KEY_SIZE];
    void* value;
};

struct dict {
    struct dict_pair** table;
    size_t table_size;
    size_t num_elements;
    const char** keys;
    bool update_keys;
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
    result->table = calloc(size, sizeof(struct dict_pair*));
    result->table_size = size;
    result->num_elements = 0;
    result->keys = NULL;
    result->update_keys = false;
    return result;
}

void dict_delete(Dict dict) {
    dict_clear(dict);
    free(dict->table);
    free(dict);
}

size_t dict_size(Dict dict) {
    return dict->num_elements;
}

void dict_set(Dict dict, const char* key, void* value, void(*destructor)(void*)) {
    size_t index = hash((unsigned char*)key) % dict->table_size;

    for (struct dict_pair* node = dict->table[index]; node; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            node->del = destructor;
            return;
        }
    }

    struct dict_pair* new_node = malloc(sizeof(struct dict_pair));
    if (new_node) {
        strncpy(new_node->key, key, DICT_MAX_KEY_SIZE);
        new_node->value = value;
        new_node->del = destructor;
        new_node->next = dict->table[index];

        dict->num_elements++;
        dict->table[index] = new_node;
        dict->update_keys = true;
    }
}

void dict_setobj(Dict dict, const char* key, void* value) {
    dict_set(dict, key, value, free);
}

void dict_setref(Dict dict, const char* key, void* value) {
    dict_set(dict, key, value, NULL);
}

void* dict_get(Dict dict, const char* key) {
    size_t index = hash((unsigned char*)key) % dict->table_size;
    struct dict_pair* node = dict->table[index];
    while(node) {
        if (strcmp(node->key, key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL;
}

void dict_remove(Dict dict, const char* key) {
    size_t index = hash((unsigned char*)key) % dict->table_size;
    struct dict_pair* a = dict->table[index], *b = a;

    while (a && strncmp(a->key, key, DICT_MAX_KEY_SIZE)) {
        b = a;
        a = a->next;
    }

    if (a) {
        b->next = a->next;
        if (a->del) a->del(a->value);
        free(a);
        dict->num_elements--;
        dict->update_keys = true;
    }
}

void dict_clear(Dict dict) {
    for (size_t i = 0; i < dict->table_size; i++) {
        struct dict_pair* curr = dict->table[i], *next;
        while (curr) {
            next = curr->next;
            if (curr->del) curr->del(curr->value);
            free(curr);
            curr = next;
        }
    }
    free(dict->keys);
    dict->keys == NULL;
    dict->update_keys = false;
    dict->num_elements = 0;
}

const char ** dict_keys(Dict dict) {
    if (dict->update_keys) {
        dict->keys = realloc(dict->keys, sizeof(char*)*dict->num_elements);

        for (size_t i = 0, curr = 0; i < dict->table_size; i++) {
            struct dict_pair* search_node = dict->table[i];
            while (search_node) {
                dict->keys[curr++] = search_node->key;
                search_node = search_node->next;
            }
        }
        dict->update_keys = false;
    }
    return dict->keys;
}