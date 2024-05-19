cmake_minimum_required(VERSION 3.28.1)

project(stoff2d)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_library(stoff2d_core STATIC
    stoff2d_core/src/stoff2d.c
    stoff2d_core/src/shader.c
    stoff2d_core/src/particle.c
    stoff2d_core/src/animation.c
    stoff2d_core/src/sprite_renderer.c
    vendor/src/cds_exlist.c
    vendor/src/cds_utils.c
    vendor/src/clm.c
    vendor/src/glad.c
    vendor/src/stbi_image.c
    )
target_include_directories(stoff2d_core PRIVATE 
    stoff2d_core/include 
    vendor/include)
target_link_directories(stoff2d_core PRIVATE vendor/lib)
target_link_libraries(stoff2d_core PRIVATE glfw3)



add_library(stoff2d_ecs STATIC
    stoff2d_ecs/src/component_map.c
    stoff2d_ecs/src/stoff2d_ecs.c
    vendor/src/cds_exlist.c
    vendor/src/cds_utils.c
    )
target_include_directories(stoff2d_ecs PRIVATE 
    stoff2d_ecs/include
    stoff2d_core/include 
    vendor/include)



add_executable(shooter
    examples/shooter/src/entities.c
    examples/shooter/src/game.c
    examples/shooter/src/main.c
    examples/shooter/src/particle_types.c
    examples/shooter/src/systems.c
    vendor/src/clm.c
    )
target_include_directories(shooter PRIVATE
    stoff2d_core/include
    stoff2d_ecs/include
    vendor/include 
    examples/shooter/include
    )
target_link_directories(shooter PRIVATE build/Debug vendor/lib)
target_link_libraries(shooter PRIVATE stoff2d_core stoff2d_ecs)

