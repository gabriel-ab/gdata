file(GLOB sources ./*.c)
file(GLOB headers ./*.h)

# add_compile_definitions(pg)
add_library(gdata ${sources} ${headers})

file(COPY ${headers} DESTINATION "include/gdata")