cmake_minimum_required(VERSION 3.10)

# Define the test executable
add_executable(fizzbuzz_tests catch_tests_fizzbuzz.cpp catch_main.cpp)

# Specify test executable's include directories
target_include_directories(fizzbuzz_tests PRIVATE ../fizzbuzz/include)
target_link_libraries(fizzbuzz_tests PRIVATE fizzbuzz)
# Register the test executable as a test
add_test(NAME FizzBuzzTests COMMAND fizzbuzz_tests)
