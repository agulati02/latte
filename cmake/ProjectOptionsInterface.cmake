# Common project settings using interface pattern
add_library(project_options INTERFACE)

#-----------------------------------------------------------
# Compiler configs
#   - Using C++20 language standard
#   - Treating warnings as errors for maintainability
#-----------------------------------------------------------

target_compile_features(project_options
    INTERFACE
        cxx_std_20
)

if(MSVC)
    target_compile_options(project_options 
        INTERFACE
            /W3
            /MP # multi-threaded build
            /WX # warnings as errors
            /ZI # program database
    )
else()
    target_compile_options(project_options 
        INTERFACE
            -Wall 
            -Wextra
            -Wpedantic
    )
endif()

# ropagate common dependencies to all consumers
target_link_libraries(project_options
    INTERFACE
        fmt::fmt
        spdlog::spdlog
)