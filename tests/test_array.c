#include <stdio.h>
#include <assert.h>
#include "array.h"

#define TEST_VALUE {1,2,3,4}

void test_array_create() {
    intArray a = array_create(sizeof(int), 4, (int[])TEST_VALUE);
    assert(a->length == 4);
    assert(a->dsize == sizeof(int));
    assert(a->at[0] == 1);
    assert(a->at[1] == 2);
    assert(a->at[2] == 3);
    assert(a->at[3] == 4);
    free(a);
}

void test_array_resize() {
    intArray a = array_create(sizeof(int), 4, (int[])TEST_VALUE);
    array_resize(a, 6);
    assert(a->length == 6);
    assert(a->dsize == sizeof(int));
    assert(a->at[0] == 1);
    assert(a->at[1] == 2);
    assert(a->at[5] == 0);
    free(a);
}

void test_array_join() {
    intArray join = ARRAY_CREATE(int, {1,2,3,4,5,6});
    intArray a = ARRAY_CREATE(int, {1,2,3});
    intArray b = ARRAY_CREATE(int, {4,5,6});
    intArray c = array_join(a,b);

    assert(array_equals(c, join) == true);
    join->at[4] = 0;
    assert(array_equals(c, join) == false);

    free(a); free(b); free(c);
    free(join);
}

void test_array_slice() {
    intArray result_true = ARRAY_CREATE(int, {3,4});
    intArray result_false = ARRAY_CREATE(int, {2,3});

    intArray a = ARRAY_CREATE(int, TEST_VALUE);
    intArray b = array_slice(a, 2, 4);

    assert(b->length == 2);
    assert(array_equals(b, result_true) == true);
    assert(array_equals(b, result_false) == false);

    free(a); free(b);
    free(result_true);
    free(result_false);
}

void test_array_equals() {
    bool equal;
    intArray a = ARRAY_CREATE(int, TEST_VALUE);
    intArray b = ARRAY_CREATE(int, TEST_VALUE);
    equal = array_equals(a, b);
    assert(equal == true);

    b->at[2] = 9;
    equal = array_equals(a, b);
    assert(equal == false);
    free(a);
    free(b);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return 1;
    }
    void (*tests[])(void) = {
        test_array_create,
        test_array_resize,
        test_array_join,
        test_array_equals,
        test_array_slice
    };
    const int n_tests = sizeof(tests)/sizeof(*tests);
    int index = atoi(argv[1]);
    if (index > -1 && index < n_tests) {
        tests[index]();
    } else {
        printf("Tests available: %i\n", n_tests);
        return 1;
    }
    return 0;
}
