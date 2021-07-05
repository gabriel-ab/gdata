find_library(CHECK_LIB check)

include_directories(../src)

file(GLOB TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)

add_executable(run_tests ${TESTS})
target_link_libraries(run_tests ${PROJECT_NAME} ${CHECK_LIB} m rt pthread subunit)
add_test(NAME test_all COMMAND run_tests)