# add new libs/modules here
add_library (DSA171A2
      "dbLib.cpp"
      "processData.cpp"
      "requestLib.cpp"
)

# add libs names here
set (allLibs "DSA171A2")

project(${PROJECT_NAME})
add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${allLibs})

set (allLibs ${allLibs} PARENT_SCOPE)