file(COPY benchmarks/ DESTINATION ./benchmarks/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_results/knapsack)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_results/tsp)
add_gtest(test_lab3 test.cpp)