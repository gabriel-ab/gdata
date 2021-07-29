#include <check.h>
#include <stdlib.h>
#include "stack.h"

#define TEST_VALUE {1,2,3,4}

START_TEST(test_stack_create) {
    Stack *stack = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    
    ck_assert_int_eq(stack->length, 4);
    ck_assert_int_eq(stack->internal.dsize, sizeof(int));
    int a = *(int*)stack->head->data;
    int b = *(int*)stack->head->next->data;
    int c = *(int*)stack->head->next->next->data;
    int d = *(int*)stack->head->next->next->next->data;
    ck_assert_int_eq(a, 4);
    ck_assert_int_eq(b, 3);
    ck_assert_int_eq(c, 2);
    ck_assert_int_eq(d, 1);
    ck_assert_ptr_eq(stack->head->next->next->next->next, NULL);
    stack_delete(stack);
} END_TEST

START_TEST(test_stack_clear) {
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    stack_clear(a);
    ck_assert_int_eq(a->length, 0);
    ck_assert_int_eq(a->internal.dsize, sizeof(int));
    ck_assert_ptr_eq(a->head, NULL);
    stack_delete(a);
} END_TEST

START_TEST(test_stack_push) {
    Stack s = {0, {sizeof(int), NULL}, NULL};
    int a = 5;
    stack_push(&s, &a);
    ck_assert_int_eq(*(int*)s.head->data, a);
    free(s.head);
} END_TEST

START_TEST(test_stack_pop) {
    Stack *s = stack_create(sizeof(int), 1, (int[]){5});
    ck_assert_int_eq(*(int*)s->head->data, 5);
    int a = *(int*)stack_pop(s);
    ck_assert_int_eq(a, 5);
    stack_delete(s);
} END_TEST

START_TEST(test_stack_to_array) {
    int array[4];
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);

    stack_to_array(s, array);
    ck_assert_int_eq(array[0], 1);
    ck_assert_int_eq(array[1], 2);
    ck_assert_int_eq(array[2], 3);
    ck_assert_int_eq(array[3], 4);
    stack_delete(s);
} END_TEST

START_TEST(test_stack_reverse) {
    int array[4];
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    stack_reverse(s);
    stack_to_array(s, array);
    ck_assert_int_eq(array[0], 4);
    ck_assert_int_eq(array[1], 3);
    ck_assert_int_eq(array[2], 2);
    ck_assert_int_eq(array[3], 1);
    stack_delete(s);
} END_TEST

START_TEST(test_stack_copy) {
    
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    Stack *b = stack_copy(a);
    bool equal = stack_equals(a,b);
    ck_assert(equal == true);
    stack_delete(a);
    stack_delete(b);
} END_TEST

START_TEST(test_stack_at) {
    int result;
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);

    result = *(int*)stack_at(s, 0);
    ck_assert_int_eq(result, 1);
    result = *(int*)stack_at(s, 3);
    ck_assert_int_eq(result, 4);
    
    stack_delete(s);
} END_TEST

START_TEST(test_stack_value) {
    int result;
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);

    result = *(int*)stack_value(s);
    ck_assert_int_eq(result, 1);

    stack_delete(s);
} END_TEST

START_TEST(test_stack_equals) {
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    Stack *b = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    
    bool equal = stack_equals(a,b);
    ck_assert(equal == true);
    stack_pop(b);
    equal = stack_equals(a,b);
    ck_assert(equal == false);

    stack_delete(a);
    stack_delete(b);
} END_TEST


Suite * stack_suite() {
    Suite* suite = suite_create("Stack Test");
    TCase* cases[] = {
        tcase_create("Core")
    };
    int num_cases = sizeof(cases)/sizeof(*cases);
    
    tcase_add_test(cases[0], test_stack_create);
    tcase_add_test(cases[0], test_stack_push);
    tcase_add_test(cases[0], test_stack_pop);
    tcase_add_test(cases[0], test_stack_clear);
    tcase_add_test(cases[0], test_stack_at);
    tcase_add_test(cases[0], test_stack_value);
    tcase_add_test(cases[0], test_stack_equals);
    tcase_add_test(cases[0], test_stack_copy);
    tcase_add_test(cases[0], test_stack_to_array);
    tcase_add_test(cases[0], test_stack_reverse);

    for (int i = 0; i < num_cases; i++)
        suite_add_tcase(suite, cases[i]);
    return suite;
}