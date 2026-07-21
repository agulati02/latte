add_library(latte_sandbox STATIC)

target_include_directories(latte_sandbox PUBLIC ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE latte_sandbox_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

target_sources(latte_sandbox PRIVATE ${latte_sandbox_SOURCES})

target_link_libraries(latte_sandbox 
    PRIVATE
        project_options
        Boost::process
)
