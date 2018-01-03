set (AVL_TEST
      "AVL.cpp"
)

set (Request_Test
      "Request1.cpp"
      "Request2.cpp"
      "Request3.cpp"
)

set (ALL_TEST
      ${AVL_TEST}
      ${Request_Test}
)

project(${TEST_NAME})
add_executable (${TEST_NAME} main-test.cpp ${ALL_TEST})
target_link_libraries(${TEST_NAME} gtest_main gtest)
target_link_libraries(${TEST_NAME} DSA171A2)
