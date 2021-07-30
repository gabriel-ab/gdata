#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

void test_list_create() {
    size_t dsize = sizeof(short);
    List *test = list_create(dsize, 0, 0);
    List expected = {.internal.dsize=dsize};

    assert(test->head == expected.head);
    assert(test->tail == expected.tail);
    assert(test->length == expected.length);

    assert(test->internal.pop == expected.internal.pop);
    assert(test->internal.dsize == expected.internal.dsize);
    free(test);
}

void test_list_pushback() {
    intList *list = LIST_CREATE(int);
    assert(list->length == 0);
    list_pushback(list, 2, (int[]){9,3});
    assert(list->length == 2);

    int first = list->head->data;
    int last = list->tail->data;

    assert(first == 9);
    assert(last == 3);
    free(list->head);
    free(list->tail);
    free(list);
}

void test_list_clear() {
    intList *list = LIST_CREATE(int, 1,2,3,4,5);
    assert(list->length == 5);
    assert(list->head != NULL);
    assert(list->tail != NULL);
    list_clear(list);
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->length == 0);
    list_delete(list);
}

void test_list_equals() {
    intList *a = LIST_CREATE(int, 1,2,3);
    intList *b = LIST_CREATE(int, 1,2);
    bool equals = list_equals(a,b);
    assert(equals == false);
    list_pushback(b, 1, (int[]){3});
    equals = list_equals(a,b);
    assert(equals == true);

    list_delete(a);
    list_delete(b);
}

void test_list_copy() {
    intList *a = LIST_CREATE(int, 1,2,3);
    intList *b = list_copy(a);
    bool equals = list_equals(a,b);
    assert(equals == true);
    list_delete(a);
    list_delete(b);
}

void test_list_resize() {
    intList *a = LIST_CREATE(int, 1,2,3);
    assert(a->length == 3);
    list_resize(a, 5);
    assert(a->length == 5);
    assert(LIST_AT(a, -1) == 0);
    list_delete(a);
}

void test_list_pushfront() {
    intList *a = LIST_CREATE(int, 1,2);
    intList *b = LIST_CREATE(int, 1,2,3,1,2);
    
    bool equals = list_equals(a,b);
    assert(equals == false);
    list_pushfront(a, 3, (int[]){1,2,3});
    equals = list_equals(a,b);
    assert(equals == true);

    list_delete(a);
    list_delete(b);
}

// UTILS
void test_list_at() {
    intList *a = LIST_CREATE(int, 1,2,3);
    int value = LIST_AT(a, 2);
    assert(value == 3);
    value = LIST_AT(a, 0);
    assert(value == 1);
    value = LIST_AT(a, -2);
    assert(value == 2);
    list_delete(a);
}

void test_list_pop() {
    intList *a = list_create(4, 5, (int[]){1,2,3,4,5});
    int value = LIST_POP(a, -1);
    assert(value == 5);
    value = LIST_POP(a, 0);
    assert(value == 1);
    value = LIST_POP(a, -2);
    assert(value == 3);
    list_delete(a);
}

void test_list_push() {
    intList *a = LIST_CREATE(int, 1,2);
    intList *b = LIST_CREATE(int, 1,3,2);
    intList *c = LIST_CREATE(int, 0,1,3,2);

    list_push(a, -2, (int[]){3});
    bool equals = list_equals(a,b);
    assert(equals == true);

    list_push(a, 0, (int[]){0});
    equals = list_equals(a,c);
    assert(equals == true);

    list_delete(a);
    list_delete(b);
    list_delete(c);
}

void test_list_slice() {
    intList *result_true = LIST_CREATE(int, 3,4);
    intList *result_false = LIST_CREATE(int, 2,3);

    intList *a = LIST_CREATE(int, 1,2,3,4);
    intList *b = list_slice(a, 2, 4);

    assert(b->length == 2);
    assert(list_equals(b, result_true) == true);
    assert(list_equals(b, result_false) == false);

    list_delete(a);
    list_delete(b);
    list_delete(result_true);
    list_delete(result_false);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return EXIT_FAILURE;
    }
    void (*tests[])(void) = {
        test_list_create,
        test_list_pushback,
        test_list_pushfront,
        test_list_clear,
        test_list_equals,
        test_list_push,
        test_list_pop,
        test_list_at,
        test_list_resize,
        test_list_copy,
        test_list_slice
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