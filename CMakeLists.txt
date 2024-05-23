cmake_minimum_required(VERSION 3.25)

project(stoff2d)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#============================================================================#
#                             clm (math library)                             # 
#============================================================================#

add_library(clm STATIC vendor/src/clm.c)

target_include_directories(clm PRIVATE include)

if (UNIX)
    target_link_libraries(clm PRIVATE m)
endif (UNIX)

#============================================================================#
#                             cds (data structures)                          # 
#============================================================================#

add_library(cds STATIC 
    vendor/src/cds_exlist.c
    vendor/src/cds_utils.c)

target_include_directories(cds PRIVATE include)

#============================================================================#
#                                stoff2d_core                                #
#============================================================================#

add_library(stoff2d_core STATIC
    stoff2d_core/stoff2d.c
    stoff2d_core/shader.c
    stoff2d_core/particle.c
    stoff2d_core/animation.c
    stoff2d_core/sprite_renderer.c
    vendor/src/glad.c
    vendor/src/stbi_image.c
    )

target_include_directories(stoff2d_core PRIVATE include vendor/include)

target_link_directories(stoff2d_core PRIVATE vendor/lib build build/Debug)

target_link_libraries(stoff2d_core PRIVATE glfw3 clm cds)

if (UNIX)
    target_link_libraries(stoff2d_core PRIVATE m)
endif (UNIX)

#=============================================================================
#                                stoff2d_ecs
#=============================================================================

add_library(stoff2d_ecs STATIC
    stoff2d_ecs/component_map.c
    stoff2d_ecs/stoff2d_ecs.c
    )

target_include_directories(stoff2d_ecs PRIVATE include)

target_link_directories(stoff2d_ecs PRIVATE build/Debug build)

target_link_libraries(stoff2d_ecs PRIVATE cds)

#=============================================================================
#                                shooter example
#=============================================================================

add_executable(shooter
    examples/shooter/src/entities.c
    examples/shooter/src/game.c
    examples/shooter/src/main.c
    examples/shooter/src/particle_types.c
    examples/shooter/src/systems.c
    )

target_include_directories(shooter PRIVATE examples/shooter/include include)

target_link_directories(shooter PRIVATE build/Debug build)

target_link_libraries(shooter PRIVATE stoff2d_core stoff2d_ecs)
