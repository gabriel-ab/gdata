#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

#define TEST_VALUE {1,2,3,4}

void test_stack_create() {
    Stack *stack = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    assert(stack->size == 4);
    assert(stack->internal.dsize == sizeof(int));
    int a = *(int*)stack->head->data;
    int b = *(int*)stack->head->next->data;
    int c = *(int*)stack->head->next->next->data;
    int d = *(int*)stack->head->next->next->next->data;
    assert(a == 4);
    assert(b == 3);
    assert(c == 2);
    assert(d == 1);
    assert(stack->head->next->next->next->next == NULL);
    stack_delete(stack);
}

void test_stack_clear() {
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    stack_clear(a);
    assert(a->size == 0);
    assert(a->internal.dsize == sizeof(int));
    assert(a->head == NULL);
    stack_delete(a);
}

void test_stack_push() {
    Stack s = {0, {sizeof(int), NULL}, NULL};
    int a = 5;
    stack_push(&s, &a);
    assert(*(int*)s.head->data == a);
    free(s.head);
}

void test_stack_pop() {
    Stack *s = stack_create(sizeof(int), 1, (int[]){5});
    assert(*(int*)s->head->data == 5);
    int a = *(int*)stack_pop(s);
    assert(a == 5);
    stack_delete(s);
}

void test_stack_to_array() {
    int array[4];
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    stack_to_array(s, array);
    assert(array[0] == 1);
    assert(array[1] == 2);
    assert(array[2] == 3);
    assert(array[3] == 4);
    stack_delete(s);
}

void test_stack_reverse() {
    int array[4];
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    stack_reverse(s);
    stack_to_array(s, array);
    assert(array[0] == 4);
    assert(array[1] == 3);
    assert(array[2] == 2);
    assert(array[3] == 1);
    stack_delete(s);
}

void test_stack_copy() {
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    Stack *b = stack_copy(a);
    bool equal = stack_equals(a,b);
    assert(equal == true);
    stack_delete(a);
    stack_delete(b);
}

void test_stack_at() {
    int result;
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    result = *(int*)stack_at(s, 0);
    assert(result == 1);
    result = *(int*)stack_at(s, 3);
    assert(result == 4);
    stack_delete(s);
}

void test_stack_value() {
    int result;
    Stack *s = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    result = *(int*)stack_value(s);
    assert(result == 1);
    stack_delete(s);
}

void test_stack_equals() {
    Stack *a = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    Stack *b = stack_create(sizeof(int), 4, (int[])TEST_VALUE);
    bool equal = stack_equals(a,b);
    assert(equal == true);
    stack_pop(b);
    equal = stack_equals(a,b);
    assert(equal == false);
    stack_delete(a);
    stack_delete(b);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return EXIT_FAILURE;
    }
    void (*tests[])(void) = {
        test_stack_create,
        test_stack_clear,
        test_stack_push,
        test_stack_pop,
        test_stack_to_array,
        test_stack_reverse,
        test_stack_copy,
        test_stack_at,
        test_stack_value,
        test_stack_equals
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