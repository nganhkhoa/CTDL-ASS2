
set (SOURCES 
      "main.cpp"
      "dbLib.cpp"
      "processData.cpp"
      "requestLib.cpp"
)

# add new libs here

# add libs names here
set (allLibs "")

if (TEST)

add_executable (${PROJECT_NAME}-test ${SOURCES} $<TARGET_OBJECTS:TESTING>)

link_directories(${google_test})
include(${google_test}/cmake/internal_utils.cmake)

cxx_library(gtest "${cxx_strict}" ${google_test}/src/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" ${google_test}/src/gtest_main.cc)

target_link_libraries(${PROJECT_NAME}-test gtest_main gtest)
target_link_libraries(${PROJECT_NAME}-test ${allLibs})
if(USEBOOST)
    file(GLOB_RECURSE boostLib "${BOOST_LIBRARYDIR}/*.a")
    target_link_libraries(${PROJECT_NAME}-test ${boostLib})
endif()

else (TEST)

add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${allLibs})
if(USEBOOST)
    file(GLOB_RECURSE boostLib "${BOOST_LIBRARYDIR}/*.a")
    target_link_libraries(${PROJECT_NAME} ${boostLib})
endif()

endif (TEST)