cmake_minimum_required(VERSION 3.10)
project(Test)
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.x
)

FetchContent_MakeAvailable(Catch2)



find_package(Catch2 REQUIRED)
include_directories(${CATCH_INCLUDE_DIR})


add_executable(tests test_ThresholdMethods.cpp ThresholdMethods.cpp)


target_link_libraries(tests Catch2::Catch2)


find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
target_link_libraries(tests ${OpenCV_LIBS})
if(OpenMP_CXX_FOUND)
    target_link_libraries(tests OpenMP::OpenMP_CXX)
endif()
