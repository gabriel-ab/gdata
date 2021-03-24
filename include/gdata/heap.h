/**
 * Generic Binary Heap Library v1.0
 * 
 * @author: Gabriel-AB
 * @github: https://github.com/Gabriel-AB/
 * 
 * Note of implementation:
 * * heap starts at[1]
 * * position at[0] is used for removed values
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_TYPEDEF(type)\
typedef struct {\
    const enum HeapOrder order;\
    struct {\
        const comparator cmp;\
        const size_t dsize;\
    } internal;\
\
    size_t max;\
    size_t used;\
    type at[];\
} *type##Heap

enum HeapOrder {
    MIN_HEAP,
    MAX_HEAP,
};

/**
 * Comparizon function, similar to `strcmp()`
 * 
 * it must return a: 
 *     value  > 0  if a > b
 *     value == 0  if a == b
 *     value  < 0  if a < b
 * 
 * with numbers is equivalent to  a - b
 */
typedef int(*comparator)(void* a,void* b);

// Generic Heap
typedef struct heap {
    const enum HeapOrder order;
    struct {
        const comparator cmp;
        const size_t dsize;
    } internal;

    size_t max;
    size_t used;
    char at[];
} *Heap;


/**
 * ### Allocate a binary heap 
 * 
 * ex: heap_create(100, sizeof(int), `Your int comparizon function`, MAX_HEAP);
 */ 
void* heap_create(size_t max_size, size_t data_size, comparator cmp, enum HeapOrder order);

// Push a new item maintaining heap structure
void heap_insert(void *heap, void *data);

/**
 *  Get and remove most relevant item
 *  Minimum for MIN_HEAP
 *  Maximum for MAX_HEAP
 */
void* heap_pop(void* heap);

// Utility functions

// number comparizon for `comparator`
int intcmp(void* a, void* b);
int floatcmp(void* a, void* b);
int doublecmp(void* a, void* b);