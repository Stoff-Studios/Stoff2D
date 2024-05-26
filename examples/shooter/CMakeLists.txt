add_executable(shooter
    src/entities.c
    src/game.c
    src/main.c
    src/particle_types.c
    src/systems.c
    )

target_include_directories(shooter PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(shooter PRIVATE stoff2d_core stoff2d_ecs)

