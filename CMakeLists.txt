cmake_minimum_required(VERSION 3.30)

# Add path to cmake configs directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Define project
project(latte VERSION 0.1.0)

include(Dependencies)   # Resolve external package dependencies
include(ProjectOptionsInterface)    # onfigure compiler options and standards

# Add subdirectory build targets
add_subdirectory(cli)
add_subdirectory(core)
add_subdirectory(tests)
