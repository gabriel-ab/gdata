cmake_minimum_required(VERSION 3.10)
project(gdata)

add_subdirectory(src gdata)
option(BUILD_TESTING "Build Tests" OFF)

if(${BUILD_TESTING})
  enable_testing()
  add_subdirectory(tests)
endif(${BUILD_TESTING})