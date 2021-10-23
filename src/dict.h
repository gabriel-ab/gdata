#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct dict* Dict;

/**
 * @brief Allocate a new dictionary (hash map)
 * 
 * @param size: maximum size of hash table
 * @return A new allocated Dictionary
 */
Dict dict_create(size_t size);


/**
 * @brief Free a dictionary and all it's contents
 * all detructor functions defined will be called
 */
void dict_delete(Dict dict);


/**
 * @brief Get number of elements defined
 * Cost: O(1)
 */
size_t dict_size(Dict dict);


/**
 * @brief Add a new key-value or update a existing one.
 * 
 * @param value pointer to value
 * @param destructor function to free this value in dict_delete()
 * if NULL, nothing is done
 */
void dict_set(Dict dict, const char* key, void* value, void(*destructor)(void*));

/**
 * @brief Add a new key-value or update a existing one
 * Simplification of dict_set() with
 * destructor == free
 * 
 * @param value pointer to value.
 * In dict_delete(), 'free()' will be called to this value.
 */
void dict_setobj(Dict dict, const char* key, void* value);

/**
 * @brief Add a new key-value or update a existing one,
 * Simplification of dict_set() with
 * destructor == NULL
 * 
 * @param value pointer to value.
 * In dict_delete() nothing is done to this value
 */
void dict_setref(Dict dict, const char* key, void* value);


/**
 * @brief Get pointer to the value of key.
 * 
 * @return pointer, cast to any type needed with *(type*).
 * returns NULL if not defined.
 */
void* dict_get(Dict dict, const char* key);

/**
 * @brief Remove all keys from the dictionary,
 * Calling all destructor functions defined.
 */
void dict_clear(Dict dict);
