cmake_minimum_required(VERSION 3.30)

project(latte VERSION 0.1.0)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Name of the final executable
add_executable(latte)

file(GLOB_RECURSE latte_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)

# Set target language standard to C++ 20
target_compile_features(latte PRIVATE cxx_std_20)

# Target sources for project
target_sources(latte PRIVATE ${latte_SOURCES})

# Simplify include structure
target_include_directories(latte PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(latte PRIVATE
        /W3
        /MP # Multi-threaded build
        /WX # warnings as errors
        /ZI # program database
    )
else()
    target_compile_options(latte PRIVATE
        -Wall 
        -Wextra
        -Wpedantic
    )
endif()


# Dependency targets

find_package(fmt CONFIG REQUIRED)

target_link_libraries(latte
    PRIVATE
        fmt::fmt
)
