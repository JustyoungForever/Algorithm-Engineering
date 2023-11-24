cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(pi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler specific extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# search for OpenMP, REQUIRED -> fatal error if not found
find_package(OpenMP REQUIRED)

# create executable with name pi
add_executable(pi main.cpp)

# link to the imported target provided by the FindOpenMP module
target_link_libraries(pi PUBLIC OpenMP::OpenMP_CXX)