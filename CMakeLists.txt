cmake_minimum_required(VERSION 3.25)

project(Stoff2D VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_EXAMPLES "build-examples" OFF)

add_subdirectory(vendor)
add_subdirectory(stoff2d_core)
add_subdirectory(stoff2d_ecs)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
