#include <check.h>
#include "array.h"

#define TEST_VALUE {1,2,3,4}

START_TEST(test_array_create) {
    intArray a = array_create(sizeof(int), 4, (int[])TEST_VALUE);
    ck_assert_int_eq(a->length, 4);
    ck_assert_int_eq(a->dsize, sizeof(int));
    ck_assert_int_eq(a->at[0], 1);
    ck_assert_int_eq(a->at[1], 2);
    ck_assert_int_eq(a->at[2], 3);
    ck_assert_int_eq(a->at[3], 4);
    free(a);
} END_TEST

START_TEST(test_array_resize) {
    intArray a = array_create(sizeof(int), 4, (int[])TEST_VALUE);
    array_resize(a, 6);
    ck_assert_int_eq(a->length, 6);
    ck_assert_int_eq(a->dsize, sizeof(int));
    ck_assert_int_eq(a->at[0], 1);
    ck_assert_int_eq(a->at[1], 2);
    ck_assert_int_eq(a->at[5], 0);
    free(a);
} END_TEST

START_TEST(test_array_join) {
    intArray join = ARRAY_CREATE(int, {1,2,3,4,5,6});
    intArray a = ARRAY_CREATE(int, {1,2,3});
    intArray b = ARRAY_CREATE(int, {4,5,6});
    intArray c = array_join(a,b);

    ck_assert(array_equals(c, join) == true);
    join->at[4] = 0;
    ck_assert(array_equals(c, join) == false);

    free(a); free(b); free(c);
    free(join);
} END_TEST

START_TEST(test_array_slice) {
    intArray result_true = ARRAY_CREATE(int, {3,4});
    intArray result_false = ARRAY_CREATE(int, {2,3});

    intArray a = ARRAY_CREATE(int, TEST_VALUE);
    intArray b = array_slice(a, 2, 4);

    ck_assert_int_eq(b->length, 2);
    ck_assert(array_equals(b, result_true) == true);
    ck_assert(array_equals(b, result_false) == false);

    free(a); free(b);
    free(result_true);
    free(result_false);
} END_TEST

START_TEST(test_array_equals) {
    bool equal;
    intArray a = ARRAY_CREATE(int, TEST_VALUE);
    intArray b = ARRAY_CREATE(int, TEST_VALUE);
    equal = array_equals(a, b);
    ck_assert(equal == true);

    b->at[2] = 9;
    equal = array_equals(a, b);
    ck_assert(equal == false);
    free(a);
    free(b);
} END_TEST

Suite * array_suite() {
    Suite* suite = suite_create("Array Test");
    TCase* cases[] = {
        tcase_create("Core")
    };
    int num_cases = sizeof(cases)/sizeof(*cases);

    tcase_add_test(cases[0], test_array_create);
    tcase_add_test(cases[0], test_array_resize);
    tcase_add_test(cases[0], test_array_join);
    tcase_add_test(cases[0], test_array_slice);
    tcase_add_test(cases[0], test_array_equals);

    for (int i = 0; i < num_cases; i++)
        suite_add_tcase(suite, cases[i]);
    
    return suite;
}