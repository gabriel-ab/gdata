cmake_minimum_required(VERSION 3.10)
project(gdata)

file(GLOB SOURCES src/*.c)
include_directories(include)
add_library(${PROJECT_NAME} STATIC ${SOURCES})