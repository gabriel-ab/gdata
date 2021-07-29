file(GLOB SOURCES ./*.c)
file(GLOB HEADERS ./*.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set(INCLUDE_FOLDER ${CMAKE_BINARY_DIR}/include/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMENT "Creating include folder at ${INCLUDE_FOLDER}"
  COMMAND ${CMAKE_COMMAND} ARGS -E
  make_directory ${INCLUDE_FOLDER}/${PROJECT_NAME})

# Getting all header files to export
foreach(file ${HEADERS})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${file} ${INCLUDE_FOLDER}/${PROJECT_NAME})
endforeach()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMENT "Exporting library to gdata.zip"
  COMMAND ${CMAKE_COMMAND} ARGS -E tar cfv
  ${CMAKE_HOME_DIRECTORY}/gdata.zip --format=zip
  $<TARGET_FILE:${PROJECT_NAME}>
  ${INCLUDE_FOLDER})