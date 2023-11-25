cmake_minimum_required(VERSION 3.10)

# Define the static library
add_library(fizzbuzz STATIC src/fizzbuzz.cpp)

# Specify library's header file directory
target_include_directories(fizzbuzz PUBLIC include)
