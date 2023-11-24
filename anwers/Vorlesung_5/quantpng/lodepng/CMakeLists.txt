cmake_minimum_required(VERSION 3.10)
project(Load)

set(LOADEPNG_FILES lodepng.cpp lodepng.h)

add_library(lodepng_lib ${LOADEPNG_FILES})