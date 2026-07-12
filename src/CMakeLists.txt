add_executable(latte)

file(GLOB_RECURSE latte_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

target_include_directories(latte PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(latte PRIVATE ${latte_SOURCES})

target_link_libraries(latte
    PRIVATE
        project_options
)