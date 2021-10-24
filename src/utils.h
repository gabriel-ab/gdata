#pragma once

/** 
 * @brief for wrapper for linked based structures (List, Stack)
 * 
 * @param cursor: variable name to use within the scope (access data: cursor->data)
 * 
 * @note use cursor->data to list's data at current iteration
 */
#define LINKED_FOR(iter, linked_structure)\
    for (__auto_type iter = linked_structure->head;\
        iter != NULL;\
        iter = iter->next)

/**
 * @brief For each on array based structures (Array, Vector, Heap)
 */
#define ARRAY_FOR(iter, array_structure)\
    for (typeof(*array_structure->at) *begin = array_structure->at,\
                             *end = begin + array_structure->length,\
                             iter = *begin;\
        begin != end; iter = *(++begin))

/**
 * @brief Allocates and define value,
 * 
 * @param type: Any type. ex: int, float, struct, union...
 * @param value: Initializer to type. 
 * 
 * @note Only for variables or structs witch sizeof() can detect it's complete size.
 */
#define newobj(type, value...) ({type __obj = (type)value, *__ptr = malloc(sizeof(__obj)); *__ptr = __obj; __ptr;})