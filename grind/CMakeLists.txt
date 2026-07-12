add_library(latte_grind STATIC)

target_include_directories(latte_grind PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE latte_grind_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

target_sources(latte_grind PRIVATE ${latte_grind_SOURCES})

target_link_libraries(latte_grind
    PRIVATE
        project_options
        cpr::cpr
        nlohmann_json::nlohmann_json
)
