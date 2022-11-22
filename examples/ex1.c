#include <list.h>
#include <utils/print.h>

int main(int argc, char const *argv[])
{
    intList a = LIST_CREATE(int, 1,2,3,4);
    list_pushback(a, 1, (int[]){300});
    print_list(a, &print_int, 0);
    list_delete(a);
    return 0;
}
