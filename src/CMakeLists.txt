# add new libs/modules here
add_library (DSA171A2
      "dbLib.cpp"
      "processData.cpp"
      "requestLib.cpp"
)

# add libs names here
set (allLibs "DSA171A2")

if (UNIT_TEST)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../test ${CMAKE_CURRENT_BINARY_DIR}/test)

else ()

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${allLibs})

endif()