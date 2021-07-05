#include <check.h>
#include <stdlib.h>

Suite * array_suite();
Suite * vector_suite();
Suite * stack_suite();
Suite * list_suite();

int main(int argc, char const *argv[])
{
    SRunner* sr = srunner_create(stack_suite());
    srunner_add_suite(sr, list_suite());
    srunner_add_suite(sr, array_suite());
    srunner_add_suite(sr, vector_suite());
    enum print_output verbosity = CK_NORMAL;
    if (argc > 1)
        verbosity = strtol(argv[1], NULL, 10);
    srunner_run_all(sr, verbosity);
    int failed = srunner_ntests_failed(sr);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
