cmake_minimum_required(VERSION 3.10)

project(gabdata)
file(GLOB SOURCES *.c)

ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SOURCES} )