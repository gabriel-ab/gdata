file(GLOB sources ./*.c)
file(GLOB headers ./*.h)

add_library(gdata STATIC ${sources} ${headers})

file(COPY ${headers} DESTINATION "include/gdata")