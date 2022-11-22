#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "vector.h"
#include "list.h"
#include "array.h"
#include "dict.h"
#include "stack.h"
#include "utils/print.h"



typedef struct {
    const char* name;
    int age;
} Person;

ARRAY_TYPEDEF(Person);

void print_person(void* person) {
    const Person* p = person;
    printf("{%s, %d}", p->name, p->age);
}


int main(int argc, char const* argv[]) {
    PersonArray a = ARRAY_CREATE(Person, {
        {"Gabriel", 22},
        {"Jon", 27}
    });
    print_array(a, print_person, P_DEFAULT);
    
    free(a);
    return 0;
}
