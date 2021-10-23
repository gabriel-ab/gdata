#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

HEAP_TYPEDEF(int);

void test_heap_create() {
    intHeap heap = heap_create(sizeof(int), 100, intcmp, MAX_HEAP);
    assert(heap->at[0] == 0);
    assert(heap->length == 0);
    assert(heap->order == MAX_HEAP);
    assert(heap->internal.dsize == sizeof(int));
    assert(heap->internal.alloc == 100);
    free(heap);
}

void test_heap_push() {
    intHeap heap = heap_create(sizeof(int), 20, intcmp, MAX_HEAP);
    heap_push(heap, (int[]){5});
    assert(heap->at[1] == 5);
    heap_push(heap, (int[]){2});
    assert(heap->at[2] == 2);
    heap_push(heap, (int[]){8});
    assert(heap->at[1] == 8);
    free(heap);
}

void test_heap_pop() {
    intHeap heap = heap_create(sizeof(int), 20, intcmp, MAX_HEAP);
    int values[] = {8, 2, 4, 10, 5, 12, 40};
    int length = sizeof(values)/sizeof(*values);
    for (int i = 0; i < length; i++)
        heap_push(heap, values + i);

    int value = *(int*)heap_pop(heap);
    assert(value == 40);
    value = *(int*)heap_pop(heap);
    assert(value == 12);
    value = *(int*)heap_pop(heap);
    assert(value == 10);
    value = *(int*)heap_pop(heap);
    assert(value == 8);
    value = *(int*)heap_pop(heap);
    assert(value == 5);
    value = *(int*)heap_pop(heap);
    assert(value == 4);
    value = *(int*)heap_pop(heap);
    assert(value == 2);
    assert(heap->length == 0);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return EXIT_FAILURE;
    }
    void (*tests[])(void) = {
        test_heap_create,
        test_heap_push,
        test_heap_pop
    };
    const int n_tests = sizeof(tests)/sizeof(*tests);
    int index = atoi(argv[1]);
    if (index > -1 && index < n_tests) {
        tests[index]();
    } else {
        printf("Tests available: %i\n", n_tests);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
