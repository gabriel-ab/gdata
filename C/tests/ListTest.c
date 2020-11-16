#define LIST_DEBUG
#include "../List.h"
#include <time.h>

char * pint(void * integer) {
    char answer[256];
    sprintf(answer, "%i", *(int*)integer);
    char * allocated_ans = malloc(strlen(answer) +1);
    strcpy(allocated_ans, answer);
    return allocated_ans;
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

    List lista = ListCreate(int);

    // TEST: pushBack
    printf("\nTEST: pushBack\n");
    for (size_t i = 0; i < 3; i++) {
        printf("pushBack(%i)\n", array[i]);
        List_pushBack(lista, List_input(int, array[i]));
    }
    printf("%s\n",List_toString(lista, pint));


    // TEST: pushFront
    printf("\nTEST: pushFront\n");
    for (size_t i = 3; i < 6; i++) {
        printf("pushFront(%i)\n", array[i]);
        List_pushFront(lista, List_input(int, array[i]));
    }
    printf("%s\n",List_toString(lista, pint));


    // TEST: popBack
    printf("\nTEST: popBack\n");
    for (size_t i = 0; i < 3; i++) {
        int *element = List_popBack(lista);
        if (element != NULL)
            printf("popBack() -> %i\n", *element);
        else 
            printf("null pointer\n");
    }
    printf("%s\n",List_toString(lista, pint));


    // TEST: popFront
    printf("\nTEST: popFront\n");
    for (size_t i = 0; i < 3; i++) {
        int *element = List_popFront(lista);
        if (element != NULL)
            printf("popfront() -> %i\n", *element);
        else 
            printf("null pointer\n");
    }
    printf("%s\n",List_toString(lista, pint));


    // TEST: fromArray
    printf("\nTEST: fromArray\n");
    List_fromArray(lista, 6, array);
    printf("%s\n",List_toString(lista, pint));


    // TEST: Resize
    printf("\nTEST: Resize\n");
    printf("resize(list[0], 8)\n");
    List_resize(lista, 8);
    printf("%s\n",List_toString(lista, pint));
    printf("resize(list[0], 5)\n");
    List_resize(lista, 5);
    printf("%s\n",List_toString(lista, pint));


    // TEST: At
    printf("\nTEST: At\n");
    int at0 = *(int*)List_at(lista,  0);
    int at1 = *(int*)List_at(lista,  1);
    int at_1 = *(int*)List_at(lista, -1);
    int at_2 = *(int*)List_at(lista, -2);
    printf("at\t0 -> %i\n", at0);
    printf("at\t1 -> %i\n", at1);
    printf("at\t-1 -> %i\n", at_1);
    printf("at\t-2 -> %i\n", at_2);

    printf("%s\n",List_toString(lista, pint));

    // TEST: forEach
    printf("\nTEST: forEach\n");
    for (int *info; info = List_forEach(lista);) {
        printf("%i\n", *info);
    }
    printf("%s\n",List_toString(lista, pint));


    // TEST: copy
    printf("\nTEST: copy\n");
    List lista2 = List_copy(lista);
    List_remove(lista2, 0);

    printf("copy(list2, list1) and remove(list2, 0)\n");
    printf("list1 -> %s",List_toString(lista, pint));
    printf("list2 -> %s",List_toString(lista2, pint));
    ListDelete(lista2);


    // TEST: remove
    printf("\nTEST: remove\n");
    List_remove(lista, 2);
    printf("index 2 removed\n");
    printf("%s\n",List_toString(lista, pint));


    // TEST: pop
    printf("\nTEST: pop\n");
    int *pop = (int*)List_pop(lista, -1);
    printf("pop(-1) -> %i\n", *pop);
    free(pop);
    printf("%s\n",List_toString(lista, pint));

    // TEST: clear
    printf("\nTEST: clear\n");
    List_clear(lista);
    printf("%s\n",List_toString(lista, pint));

    // TEST: pushArray
    printf("\nTEST: pushArray\n");
    List_pushArray(lista, 6, array);
    printf("%s\n",List_toString(lista, pint));

    // TEST: toArray
    printf("\nTEST: toArray:\n");
    int* arr = List_toArray(lista);

    printf("array = [ ");
    for (size_t i = 0; i < (lista)->size; i++)
        printf("%i ", arr[i]);
    printf("]\n");
    
    ListDelete(lista);

    printf("\nALL TESTS PASSED!\n");
    return 0;
}
