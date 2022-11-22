cmake_minimum_required(VERSION 3.10)
project(gdata)

add_subdirectory(src gdata)
option(BUILD_TESTING "Build Tests" OFF)
option(BUILD_EXAMPLES "Build Examples" OFF)
option(BUILD_SHARED_LIBS "Build Shared lib files" OFF)

if(${BUILD_TESTING})
enable_testing()
add_subdirectory(tests)
endif(${BUILD_TESTING})

if(${BUILD_EXAMPLES})
add_subdirectory(examples)
endif(${BUILD_EXAMPLES})
