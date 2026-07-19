add_library(latte_cli STATIC)

target_include_directories(latte_cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE latte_cli_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

target_sources(latte_cli PRIVATE ${latte_cli_SOURCES})

target_link_libraries(latte_cli 
    PRIVATE 
        project_options
        CLI11::CLI11
        latte_grind
        latte_sandbox
)
