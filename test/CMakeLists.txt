set (AVL_TEST
      "AVL.cpp"
)

set (ALL_TEST
      ${AVL_TEST}
)

# build googletest from source
set (google_test "${PROJECT_ROOT_DIR}/include/googletest/googletest")
include_directories(${google_test})
link_directories(${google_test})
include(${google_test}/cmake/internal_utils.cmake)
cxx_library(gtest "${cxx_strict}" ${google_test}/src/gtest-all.cc)
cxx_library(gtest_main "${cxx_strict}" ${google_test}/src/gtest_main.cc)


project(${TEST_NAME})
add_executable (${TEST_NAME} main.cpp ${ALL_TEST})
target_link_libraries(${TEST_NAME} gtest_main gtest)
target_link_libraries(${TEST_NAME} ${allLibs})