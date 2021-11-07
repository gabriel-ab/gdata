#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"

#define TEST_VALUE 1,2,3,4

void test_vector_create() {
    intVector a = vector_create(sizeof(int), 4, (int[]){TEST_VALUE});
    assert(a->size == 4);
    assert(a->internal.dsize == sizeof(int));
    assert(a->at[0] == 1);
    assert(a->at[1] == 2);
    assert(a->at[2] == 3);
    assert(a->at[3] == 4);
    vector_delete(a);
}

void test_vector_pushback() {
    intVector vector = vector_create(4, 0, 0);
    assert(vector->size == 0);
    vector_pushback(vector, 2, (int[]){9,3});

    assert(vector->size == 2);
    assert(vector->at[0] == 9);
    assert(vector->at[1] == 3);

    vector_delete(vector);
}

void test_vector_equals() {
    bool equal;
    intVector a = VECTOR_CREATE(int, TEST_VALUE);
    intVector b = VECTOR_CREATE(int, TEST_VALUE);
    equal = vector_equals(a, b);
    assert(equal == true);

    b->at[2] = 9;
    equal = vector_equals(a, b);
    assert(equal == false);
    vector_delete(a);
    vector_delete(b);
}

void test_vector_slice() {
    intVector result_true = VECTOR_CREATE(int, 3,4);
    intVector result_false = VECTOR_CREATE(int, 2,3);

    intVector a = VECTOR_CREATE(int, TEST_VALUE);
    intVector b = vector_slice(a, 2, 4);

    assert(b->size == 2);
    assert(vector_equals(b, result_true) == true);
    assert(vector_equals(b, result_false) == false);

    vector_delete(a);
    vector_delete(b);
    vector_delete(result_true);
    vector_delete(result_false);
}

void test_vector_copy() {
    intVector a = vector_create(4, 3, (int[]){1,2,3});
    intVector b = vector_copy(a);
    bool equals = vector_equals(a,b);
    assert(equals == true);
    vector_delete(a);
    vector_delete(b);
}

void test_vector_remove() {
    intVector a = VECTOR_CREATE(int, 1,2,3,4,5);
    intVector b = VECTOR_CREATE(int, 1,2,3,5);
    intVector c = VECTOR_CREATE(int, 1,3,5);
    vector_remove(a, 3);
    bool equal = vector_equals(a,b);
    assert(equal == true);
    vector_remove(a, 1);
    equal = vector_equals(a,c);
    assert(equal == true);
    vector_delete(a);
}

void test_vector_pushfront() {
    intVector a = VECTOR_CREATE(int, 1,2);
    intVector b = VECTOR_CREATE(int, 1,2,3,1,2);
    
    bool equals = vector_equals(a,b);
    assert(equals == false);
    vector_pushfront(a, 3, (int[]){1,2,3});
    equals = vector_equals(a,b);
    assert(equals == true);
    vector_delete(a);
    vector_delete(b);
}

void test_vector_popback() {
    intVector a = VECTOR_CREATE(int, TEST_VALUE);
    intVector b = VECTOR_CREATE(int, 1,2,3);
    assert(vector_equals(a,b) == false);
    int value = VECTOR_POPBACK(a);
    assert(value == 4);
    assert(vector_equals(a,b) == true);
    value = VECTOR_POPBACK(a);
    assert(value == 3);
    vector_delete(a);
    vector_delete(b);
}

void test_vector_popfront() {
    intVector a = VECTOR_CREATE(int, TEST_VALUE);
    intVector b = VECTOR_CREATE(int, 2,3,4);
    assert(vector_equals(a,b) == false);
    int value = VECTOR_POPFRONT(a);
    assert(value == 1);
    assert(vector_equals(a,b) == true);
    value = VECTOR_POPFRONT(a);
    assert(value == 2);
    vector_delete(a);
    vector_delete(b);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return EXIT_FAILURE;
    }
    void (*tests[])(void) = {
        test_vector_create,
        test_vector_pushback,
        test_vector_equals,
        test_vector_slice,
        test_vector_copy,
        test_vector_remove,
        test_vector_pushfront,
        test_vector_popback,
        test_vector_popfront
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