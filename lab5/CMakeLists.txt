file(COPY benchmarks/ DESTINATION ./benchmarks/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_results/)
add_gtest(test_lab5 test.cpp)