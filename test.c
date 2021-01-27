#include <stdio.h>
#include "gdata/array.h"
#include "gdata/list.h"
#include "print.h"


int main(void) {
    List lista = list_create(int);
    list_push_array(lista, 10, (int[10]){
        1,2,3,4,5,9,3,2,192,3
    });

    list_delete(lista);
    return 0;
}