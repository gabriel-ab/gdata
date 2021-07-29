#include <check.h>
#include <stdlib.h>
#include "list.h"

// CORE
START_TEST(test_list_create) {
    size_t dsize = sizeof(short);
    List *test = list_create(dsize, 0, 0);
    List expected = {.internal.dsize=dsize};

    ck_assert_ptr_eq(test->head, expected.head);
    ck_assert_ptr_eq(test->tail, expected.tail);
    ck_assert_int_eq(test->length, expected.length);

    ck_assert_ptr_eq(test->internal.pop, expected.internal.pop);
    ck_assert_int_eq(test->internal.dsize, expected.internal.dsize);
    free(test);
} END_TEST

START_TEST(test_list_pushback) {
    intList *list = LIST_CREATE(int);
    ck_assert_int_eq(list->length, 0);
    list_pushback(list, 2, (int[]){9,3});
    ck_assert_int_eq(list->length, 2);

    int first = list->head->data;
    int last = list->tail->data;

    ck_assert_int_eq(first, 9);
    ck_assert_int_eq(last, 3);
    free(list->head);
    free(list->tail);
    free(list);
} END_TEST

START_TEST(test_list_clear) {
    intList *list = LIST_CREATE(int, 1,2,3,4,5);
    ck_assert_int_eq(list->length, 5);
    ck_assert_ptr_ne(list->head, NULL);
    ck_assert_ptr_ne(list->tail, NULL);
    list_clear(list);
    ck_assert_ptr_eq(list->head, NULL);
    ck_assert_ptr_eq(list->tail, NULL);
    ck_assert_int_eq(list->length, 0);
    list_delete(list);
} END_TEST

START_TEST(test_list_equals) {
    intList *a = LIST_CREATE(int, 1,2,3);
    intList *b = LIST_CREATE(int, 1,2);
    bool equals = list_equals(a,b);
    ck_assert_int_eq(equals, false);
    list_pushback(b, 1, (int[]){3});
    equals = list_equals(a,b);
    ck_assert_int_eq(equals, true);

    list_delete(a);
    list_delete(b);
} END_TEST

START_TEST(test_list_copy) {
    intList *a = LIST_CREATE(int, 1,2,3);
    intList *b = list_copy(a);
    bool equals = list_equals(a,b);
    ck_assert_int_eq(equals, true);
    list_delete(a);
    list_delete(b);
} END_TEST

START_TEST(test_list_resize) {
    intList *a = LIST_CREATE(int, 1,2,3);
    ck_assert_int_eq(a->length, 3);
    list_resize(a, 5);
    ck_assert_int_eq(a->length, 5);
    ck_assert_int_eq(LIST_AT(a, -1), 0);
    list_delete(a);
} END_TEST

START_TEST(test_list_pushfront) {
    intList *a = LIST_CREATE(int, 1,2);
    intList *b = LIST_CREATE(int, 1,2,3,1,2);
    
    bool equals = list_equals(a,b);
    ck_assert_int_eq(equals, false);
    list_pushfront(a, 3, (int[]){1,2,3});
    equals = list_equals(a,b);
    ck_assert_int_eq(equals, true);

    list_delete(a);
    list_delete(b);
} END_TEST

// UTILS
START_TEST(test_list_at) {
    intList *a = LIST_CREATE(int, 1,2,3);
    int value = LIST_AT(a, 2);
    ck_assert_int_eq(value, 3);
    value = LIST_AT(a, 0);
    ck_assert_int_eq(value, 1);
    value = LIST_AT(a, -2);
    ck_assert_int_eq(value, 2);
    list_delete(a);
} END_TEST

START_TEST(test_list_pop) {
    intList *a = list_create(4, 5, (int[]){1,2,3,4,5});
    int value = LIST_POP(a, -1);
    ck_assert_int_eq(value, 5);
    value = LIST_POP(a, 0);
    ck_assert_int_eq(value, 1);
    value = LIST_POP(a, -2);
    ck_assert_int_eq(value, 3);
    list_delete(a);
} END_TEST

START_TEST(test_list_push) {
    intList *a = LIST_CREATE(int, 1,2);
    intList *b = LIST_CREATE(int, 1,3,2);
    intList *c = LIST_CREATE(int, 0,1,3,2);

    list_push(a, -2, (int[]){3});
    bool equals = list_equals(a,b);
    ck_assert_int_eq(equals, true);

    list_push(a, 0, (int[]){0});
    equals = list_equals(a,c);
    ck_assert_int_eq(equals, true);

    list_delete(a);
    list_delete(b);
    list_delete(c);
} END_TEST

START_TEST(test_list_slice) {
    intList *result_true = LIST_CREATE(int, 3,4);
    intList *result_false = LIST_CREATE(int, 2,3);

    intList *a = LIST_CREATE(int, 1,2,3,4);
    intList *b = list_slice(a, 2, 4);

    ck_assert_int_eq(b->length, 2);
    ck_assert(list_equals(b, result_true) == true);
    ck_assert(list_equals(b, result_false) == false);

    list_delete(a);
    list_delete(b);
    list_delete(result_true);
    list_delete(result_false);
} END_TEST

Suite * list_suite() {
    Suite* suite = suite_create("List Test");
    TCase* cases[] = {
        tcase_create("Core"),
        tcase_create("Utils")
    };
    int num_cases = sizeof(cases)/sizeof(*cases);

    tcase_add_test(cases[0], test_list_create);
    tcase_add_test(cases[0], test_list_pushback);
    tcase_add_test(cases[0], test_list_pushfront);
    tcase_add_test(cases[0], test_list_clear);
    tcase_add_test(cases[0], test_list_equals);
    tcase_add_test(cases[0], test_list_push);
    tcase_add_test(cases[0], test_list_pop);
    tcase_add_test(cases[0], test_list_at);
    tcase_add_test(cases[0], test_list_resize);
    tcase_add_test(cases[0], test_list_copy);
    tcase_add_test(cases[0], test_list_slice);

    for (int i = 0; i < num_cases; i++)
        suite_add_tcase(suite, cases[i]);
    
    return suite;
}