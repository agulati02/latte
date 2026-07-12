cmake_minimum_required(VERSION 3.30)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(latte VERSION 0.1.0)

include(Dependencies)
include(ProjectOptionsInterface)

add_subdirectory(src)
add_subdirectory(tests)
