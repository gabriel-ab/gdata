/**
 * Generic Binary Heap
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
typedef struct type##_heap {\
    size_t length;\
    const enum HeapOrder order;\
    struct {\
        size_t alloc;\
        const comparator cmp;\
        const size_t dsize;\
    } internal;\
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
    size_t length;
    const enum HeapOrder order;
    const struct {
        size_t alloc;
        comparator cmp;
        size_t dsize;
    } internal;
    char at[];
} Heap;

/**
 * @brief Allocate a binary heap 
 * 
 * @param size: Maximum number of elements
 * @param dsize: Data size, in bytes. normally given by sizeof operator
 * @param cmp: Comparator function, similar to strcmp, take references to
 * two values and outputs a integer. see: comparator
 * @param order: see enum HeapOrder 
 */ 
void* heap_create(size_t size, size_t dsize, comparator cmp, enum HeapOrder order);

/**
 * @brief Push a new item maintaining heap structure
 * 
 * @param heap: Any kind of Heap. ex: intHeap, floatHeap, ...
 * @param data: Reference to value
 */
void heap_push(void *heap, void *data);

/**
 * @brief Get and remove most relevant item.
 * Minimum for MIN_HEAP, Maximum for MAX_HEAP
 * 
 * @param heap: Any kind of Heap. ex: intHeap, floatHeap, ...
 * @return reference to value removed (will last until next call)
 * 
 */
void* heap_pop(void* heap);

// Utility functions

// number comparizon for `comparator`
int intcmp(void* a, void* b);
int floatcmp(void* a, void* b);
int doublecmp(void* a, void* b);