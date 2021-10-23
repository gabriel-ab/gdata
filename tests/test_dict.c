#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"

struct dict_pair {
    struct dict_pair* next;
    void(*del)(void*);
    char key[32];
    void* value;
};

struct dict {
    struct dict_pair** children;
    size_t length;
    size_t max_size;
};


void test_dict_creation_and_deletion() {
    Dict d = dict_create(10);
    assert(d != NULL);
    assert(d->length == 0);
    assert(d->max_size == 10);
    dict_delete(d);
}


static void fake_free_str(void* p) {
    assert(strcmp(p, "value") == 0);
}

static void fake_free_int(void* p) {
    assert(10 == *(int*)p);
}

void test_dict_set_adding() {
    Dict d = dict_create(10);
    int a = 10;
    const char str[] = "value";
    dict_set(d, "str", (void*)str, fake_free_str);
    dict_set(d, "int", &a, fake_free_int);
    dict_set(d, "int2", (int[]){10}, fake_free_int);
    assert(d->length == 3);
    dict_delete(d);
}


void test_dict_sets() {
    Dict d = dict_create(10);
    int a = 10;
    dict_setref(d, "ref", &a);

    int* b = malloc(sizeof(int));
    *b = 20;
    dict_setobj(d, "obj", b);
    
    dict_delete(d);
}


void test_dict_set_updating() {
    Dict d = dict_create(10);
    int a = 10, b = 5;
    
    dict_setref(d, "int", &a);
    assert(&a == (int*)dict_get(d, "int"));
    assert(10 == *(int*)dict_get(d, "int"));

    dict_setref(d, "int", &b);
    assert(&b == (int*)dict_get(d, "int"));
    assert(5 == *(int*)dict_get(d, "int"));

    assert(d->length == 1);
    dict_delete(d);
}


void test_dict_get() {
    Dict d = dict_create(10);
    int in, out;

    in == 5;
    dict_setref(d, "5", &in);
    out = *(int*)dict_get(d, "5");
    assert(in == out);

    in = 2;
    dict_setref(d, "2", &in);
    out = *(int*)dict_get(d, "2");
    assert(in == out);

    assert(d->length == 2);
    dict_delete(d);
}


int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <id>\n", argv[0]);
        return EXIT_FAILURE;
    }
    void (*tests[])(void) = {
        test_dict_creation_and_deletion,
        test_dict_set_adding,
        test_dict_sets,
        test_dict_get,
        test_dict_set_updating
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
