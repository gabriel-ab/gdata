#include <check.h>
#include "vector.h"

#define TEST_VALUE 1,2,3,4

START_TEST(test_vector_create) {
    intVector *a = vector_create(sizeof(int), 4, (int[]){TEST_VALUE});
    ck_assert_int_eq(a->length, 4);
    ck_assert_int_eq(a->internal.dsize, sizeof(int));
    ck_assert_int_eq(a->at[0], 1);
    ck_assert_int_eq(a->at[1], 2);
    ck_assert_int_eq(a->at[2], 3);
    ck_assert_int_eq(a->at[3], 4);
    vector_delete(a);
} END_TEST

START_TEST(test_vector_pushback) {
    intVector *vector = vector_create(4, 0, 0);
    ck_assert_int_eq(vector->length, 0);
    vector_pushback(vector, 2, (int[]){9,3});

    ck_assert_int_eq(vector->length, 2);
    ck_assert_int_eq(vector->at[0], 9);
    ck_assert_int_eq(vector->at[1], 3);

    vector_delete(vector);
} END_TEST

START_TEST(test_vector_equals) {
    bool equal;
    intVector *a = VECTOR_CREATE(int, TEST_VALUE);
    intVector *b = VECTOR_CREATE(int, TEST_VALUE);
    equal = vector_equals(a, b);
    ck_assert(equal == true);

    b->at[2] = 9;
    equal = vector_equals(a, b);
    ck_assert(equal == false);
    vector_delete(a);
    vector_delete(b);
} END_TEST

START_TEST(test_vector_slice) {
    intVector *result_true = VECTOR_CREATE(int, 3,4);
    intVector *result_false = VECTOR_CREATE(int, 2,3);

    intVector *a = VECTOR_CREATE(int, TEST_VALUE);
    intVector *b = vector_slice(a, 2, 4);

    ck_assert_int_eq(b->length, 2);
    ck_assert(vector_equals(b, result_true) == true);
    ck_assert(vector_equals(b, result_false) == false);

    vector_delete(a);
    vector_delete(b);
    vector_delete(result_true);
    vector_delete(result_false);
} END_TEST

START_TEST(test_vector_copy) {
    intVector *a = vector_create(4, 3, (int[]){1,2,3});
    intVector *b = vector_copy(a);
    bool equals = vector_equals(a,b);
    ck_assert_int_eq(equals, true);
    vector_delete(a);
    vector_delete(b);
} END_TEST

START_TEST(test_vector_remove) {
    intVector *a = VECTOR_CREATE(int, 1,2,3,4,5);
    intVector *b = VECTOR_CREATE(int, 1,2,3,5);
    intVector *c = VECTOR_CREATE(int, 1,3,5);
    vector_remove(a, 3);
    bool equal = vector_equals(a,b);
    ck_assert_int_eq(equal, true);
    vector_remove(a, 1);
    equal = vector_equals(a,c);
    ck_assert_int_eq(equal, true);
    vector_delete(a);
} END_TEST

START_TEST(test_vector_pushfront) {
    intVector *a = VECTOR_CREATE(int, 1,2);
    intVector *b = VECTOR_CREATE(int, 1,2,3,1,2);
    
    bool equals = vector_equals(a,b);
    ck_assert_int_eq(equals, false);
    vector_pushfront(a, 3, (int[]){1,2,3});
    equals = vector_equals(a,b);
    ck_assert_int_eq(equals, true);
    vector_delete(a);
    vector_delete(b);
} END_TEST

START_TEST(test_vector_popback) {
    intVector *a = VECTOR_CREATE(int, TEST_VALUE);
    intVector *b = VECTOR_CREATE(int, 1,2,3);
    ck_assert_int_eq(vector_equals(a,b), false);
    int value = VECTOR_POPBACK(a);
    ck_assert_int_eq(value, 4);
    ck_assert_int_eq(vector_equals(a,b), true);
    value = VECTOR_POPBACK(a);
    ck_assert_int_eq(value, 3);
    vector_delete(a);
    vector_delete(b);
} END_TEST

START_TEST(test_vector_popfront) {
    intVector *a = VECTOR_CREATE(int, TEST_VALUE);
    intVector *b = VECTOR_CREATE(int, 2,3,4);
    ck_assert_int_eq(vector_equals(a,b), false);
    int value = VECTOR_POPFRONT(a);
    ck_assert_int_eq(value, 1);
    ck_assert_int_eq(vector_equals(a,b), true);
    value = VECTOR_POPFRONT(a);
    ck_assert_int_eq(value, 2);
    vector_delete(a);
    vector_delete(b);
} END_TEST

Suite * vector_suite() {
    Suite* suite = suite_create("Vector Test");
    TCase* cases[] = {
        tcase_create("Core")
    };
    int num_cases = sizeof(cases)/sizeof(*cases);
   
    tcase_add_test(cases[0], test_vector_create);
    tcase_add_test(cases[0], test_vector_pushback);
    tcase_add_test(cases[0], test_vector_equals);
    tcase_add_test(cases[0], test_vector_slice);
    tcase_add_test(cases[0], test_vector_copy);
    tcase_add_test(cases[0], test_vector_remove);
    tcase_add_test(cases[0], test_vector_pushfront);
    tcase_add_test(cases[0], test_vector_popback);
    tcase_add_test(cases[0], test_vector_popfront);
   
    for (int i = 0; i < num_cases; i++)
        suite_add_tcase(suite, cases[i]);
    
    return suite;
}