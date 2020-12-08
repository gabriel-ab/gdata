#define LIST_DEBUG
#include "../List.h"
#include <stdio.h>
#include <time.h>

void print(List list) {
    printf("[ ");
    for (int *integer; integer = List_forEach(list);)
        printf("%i ", *integer);
    printf("]\n");
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    int array[] = {
        rand() % 100,
        rand() % 100,
        rand() % 100,
        rand() % 100,
        rand() % 100,
        rand() % 100
    };

    List lista = ListCreate(int, {});
    print(lista);

    // TEST: pushBack
    printf("\nTEST: pushBack\n");
    for (size_t i = 0; i < 3; i++) {
        printf("pushBack(%i)\n", array[i]);
        List_pushBack(lista, &array[i]);
    }
    print(lista);
    
    // TEST: pushFront
    printf("\nTEST: pushFront\n");
    for (size_t i = 3; i < 6; i++) {
        printf("pushFront(%i)\n", array[i]);
        List_pushFront(lista, &array[i]);
    }
    print(lista);

    // TEST: popBack
    printf("\nTEST: popBack\n");
    for (size_t i = 0; i < 3; i++) {
        int *element = List_popBack(lista);
        if (element != NULL)
            printf("popBack() -> %i\n", *element);
        else 
            printf("null pointer\n");
    }
    print(lista);

    // TEST: popFront
    printf("\nTEST: popFront\n");
    for (size_t i = 0; i < 3; i++) {
        int *element = List_popFront(lista);
        if (element != NULL) {
            printf("popfront() -> %i\n", *element);
            free(element);
        }
        else 
            printf("null pointer\n");
    }
    print(lista);

    // TEST: Create as array
    printf("\nTEST: Create as array\n");
    ListDelete(&lista);
    lista = ListCreate_as(int, array, 6);
    print(lista);

    // TEST: Resize
    printf("\nTEST: Resize\n");
    printf("resize(list[0], 5)\n");
    List_resize(lista, 5);
    print(lista);

    // TEST: At
    printf("\nTEST: At\n");
    int *at0 = List_at(lista,  0);
    int *at1 = List_at(lista,  1);
    int *at_1 = List_at(lista, -1);
    int *at_2 = List_at(lista, -2);
    printf("at\t0 -> %i\n", *at0);
    printf("at\t1 -> %i\n", *at1);
    printf("at\t-1 -> %i\n", *at_1);
    printf("at\t-2 -> %i\n", *at_2);
    print(lista);

    // TEST: forEach
    printf("\nTEST: forEach\n");
    for (int *info; info = List_forEach(lista);) {
        printf("item: %i\n", *info);
    }
    print(lista);

    // TEST: copy
    printf("\nTEST: copy\n");
    List lista2 = List_copy(lista);
    List_remove(lista2, 0);
    printf("copy(list2, list1) and remove(list2, 0)\n");
    print(lista2);
    ListDelete(&lista2);
    print(lista);


    // TEST: remove
    printf("\nTEST: remove\n");
    List_remove(lista, 2);
    printf("index 2 removed\n");
    print(lista);

    // TEST: pop
    printf("\nTEST: pop\n");
    int *pop = (int*)List_pop(lista, -1);
    printf("pop(-1) -> %i\n", *pop);
    print(lista);free(pop);
    
    // TEST: clear
    printf("\nTEST: clear\n");
    List_clear(lista);
    print(lista);
    
    // TEST: pushArray
    printf("\nTEST: pushArray\n");
    List_pushArray(lista, 6, array);
    print(lista);
    
    // TEST: toArray
    printf("\nTEST: toArray:\n");
    int* arr = List_toArray(lista);

    printf("array = [ ");
    for (size_t i = 0; i < (lista)->size; i++)
        printf("%i ", arr[i]);
    printf("]\n");
    
    ListDelete(&lista);

    printf("\nALL TESTS PASSED!\n");
    return 0;
}
