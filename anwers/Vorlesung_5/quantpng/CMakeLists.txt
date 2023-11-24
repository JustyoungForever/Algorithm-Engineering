cmake_minimum_required(VERSION 3.10)
project(qunt)

add_subdirectory(lodepng)
add_subdirectory(quantize)

add_executable(qunt.exe main.cpp)
target_link_libraries(qunt.exe lodepng_lib Source_lib)
