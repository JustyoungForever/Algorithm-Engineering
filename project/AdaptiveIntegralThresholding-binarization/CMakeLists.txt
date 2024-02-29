cmake_minimum_required(VERSION 3.10)

# Set the project name
project(ThresholdProject)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Find the OpenMP package
find_package(OpenMP REQUIRED)

# Include directories for OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable for the main application
add_executable(ThresholdApp main.cpp ThresholdMethods.cpp)

# Link the OpenCV libraries to the executable
target_link_libraries(ThresholdApp ${OpenCV_LIBS})

# If OpenMP was found, link it to the target
if(OpenMP_CXX_FOUND)
    target_link_libraries(ThresholdApp OpenMP::OpenMP_CXX)
endif()

# Set the compile options based on the compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ThresholdApp PRIVATE -O3 -march=native)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(ThresholdApp PRIVATE /O2)
endif()