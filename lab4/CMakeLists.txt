file(COPY benchmarks/ DESTINATION ./benchmarks/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_results/ls)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_results/ils)
add_gtest(test_lab4 test.cpp)