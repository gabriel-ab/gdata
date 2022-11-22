#include "../list.h"
#include "../array.h"
#include "../stack.h"
#include "../heap.h"
#include "../vector.h"
#include "../dict.h"
#include <stdbool.h>


enum printFlag {
    P_DEFAULT   = 0,
    P_SHORT     = 1 << 0,
    P_INFO      = 1 << 1,
    P_LONG      = 1 << 2,
    P_DEBUG     = 1 << 3
};

// PRINT FUNCTIONS

typedef void (*print_primitive)(const void* primitive);
typedef void (*print_container)(const void* container, print_primitive printer, enum printFlag flags);

// print integer
void print_int(const void* primitive) {
    printf("%i", primitive ? *(int*)primitive : 0);
}

// print float
void print_float(const void* primitive) {
    printf("%.2f", primitive ? *(float*)primitive : 0.0f);
}

// print char
void print_char(const void* primitive) {
    printf("%c", primitive ? *(char*)primitive : '\0');
}
// print string
void print_string(const void* primitive) {
    printf("\"%s\"", primitive ? (char*)primitive : "\0");
}

void print(const char* name, void* data, size_t size, size_t dsize, void (*print_item)(void*), enum printFlag flags) {
    bool all = (flags & P_LONG) || (size < 32);
    bool simple = flags & P_SHORT;
    bool jump = size > 16;
    const char jump_key = jump ? '\n' : ' ';
    const char indent_key = jump ? '\t' : '\0';
    
    if (!simple)
        printf("%s (", name);
    printf("[");

    size_t elements_printed = all ? size : 3;

    for (size_t i = 0; i < elements_printed; i++) {
        printf("%c%c", jump_key, indent_key);
        print_item(data + i*dsize);
    }

    if (!all) {
        printf("%c%c...%c%c", jump_key, indent_key, jump_key, indent_key);
        print_item(data + dsize * (size -1));
    }

    if (simple) printf("%c]\n", jump_key);
    else printf("%c], size: %lu)\n", jump_key, size);
}

void print_array(void* array, void(*print_item)(void* data), enum printFlag flags) {
    charArray const arr = array;
    print("Array", arr->at, arr->size, arr->internal.dsize, print_item, flags);
}

void print_vector(void* vector, void(*print_item)(void* data), enum printFlag flags) {
    charVector V = vector;
    print("Vector", V->at, V->size, V->internal.dsize, print_item, flags);
    
    if (flags & P_INFO) {
        size_t allocated = V->internal.alloc*V->internal.dsize;
        printf("-- info (allocated: %lu (%lu bytes), dsize: %lu)\n",
                V->internal.alloc, allocated, V->internal.dsize);
    }

    if (flags & P_DEBUG) {
        print("-- debug", V->internal.begin, V->internal.alloc, V->internal.dsize, print_item, flags);
        size_t l = V->internal.offset;
        size_t r = V->internal.alloc - V->internal.offset - V->size;
        printf("-- free space (left: %lu, right: %lu)\n", l, r);
    }
}

void print_stack(void* stack, void(*print_item)(void* data), enum printFlag flags) {
    Stack *S = stack;
    char data[S->internal.dsize * S->size];
    stack_to_array(S, data);
    print("Stack", data, S->size, S->internal.dsize, print_item, flags);
}

void print_list(void* list, void(*print_item)(void* data), enum printFlag flags) {
    List* L = list;
    char data[L->internal.dsize * L->size];
    list_to_array(L, data);
    print("List", data, L->size, L->internal.dsize, print_item, flags);

    if (flags & P_INFO) {
        size_t alloc = L->size * (2 * sizeof(void*) + L->internal.dsize) + sizeof(List);
        printf("List.info: (allocated: %lu bytes, dsize: %lu)\n", alloc, L->internal.dsize);
    }
    if (flags & P_DEBUG) {
        printf("List.debug (head: %p, tail: %p, pop:%p)\n", 
            L->head, L->tail, L->internal.pop);
    }
}

void print_heap(void* heap, void (*print_item)(void *data), enum printFlag flags) {
    Heap H = heap;
    const char *kind[] = {"Min","Max"};
    char title[10] = "";
    strcat(title, kind[H->order]);
    strcat(title, "Heap");
    print(title, H->at, H->size, H->internal.dsize, print_item, flags);

    if (flags & P_INFO) {
        printf("-- info (used: %lu/%lu, comparison function at: %p)\n", 
            H->size, H->internal.alloc, H->internal.cmp);
    }

    if (flags & P_DEBUG) {
        print("-- debug", H->at, H->internal.alloc, H->internal.dsize, print_item, flags);
    }

}

void print_dict(Dict dict, enum printFlag flags) {
    bool all = (flags & P_LONG) || (dict_size(dict) < 32);
    bool simple = flags & P_SHORT;
    bool jump = true;
    const char jump_key = jump ? '\n' : ' ';
    const char indent_key = jump ? '\t' : '\0';
    const char ** keys = dict_keys(dict);
    printf("dict (%c", jump_key);
    for (size_t i = 0; i < dict_size(dict); i++) {
        printf("%10.64s : %p,%c", keys[i], dict_get(dict, keys[i]), jump_key);
    }
    printf(")%c", jump_key);
}