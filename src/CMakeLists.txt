file(GLOB SOURCES ./*.c)
file(GLOB HEADERS ./*.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMENT "Creating folder include/gdata/"
  COMMAND ${CMAKE_COMMAND} ARGS -E
  make_directory ${CMAKE_CURRENT_BINARY_DIR}/include/gdata)
  
# Getting all header files to export
foreach(file ${HEADERS})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${file} ${CMAKE_CURRENT_BINARY_DIR}/include/gdata)
endforeach()
  
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMENT "Exporting gdata.zip"
  COMMAND ${CMAKE_COMMAND} ARGS -E tar cfv 
    ${CMAKE_HOME_DIRECTORY}/gdata.zip --format=zip 
    ${CMAKE_CURRENT_BINARY_DIR}/libgdata.a
    ${CMAKE_CURRENT_BINARY_DIR}/include)
