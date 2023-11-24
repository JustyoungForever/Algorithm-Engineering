cmake_minimum_required(VERSION 3.10)
project(Source)
file(GLOB QUANTIZE_SOURCES "*.c")
file(GLOB QUANTIZE_HEADERS "*.h")

add_library(Source_lib ${QUANTIZE_SOURCES} ${QUANTIZE_HEADERS})