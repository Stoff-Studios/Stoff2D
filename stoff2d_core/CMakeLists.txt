add_library(stoff2d_core
    src/animation.c
    src/glad.c
    src/particle.c
    src/shader.c
    src/sprite_renderer.c
    src/stbi_image.c
    src/stoff2d.c)

target_include_directories(stoff2d_core PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor"
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(stoff2d_core PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(stoff2d_core PRIVATE glfw cds)

target_link_libraries(stoff2d_core PUBLIC clm)
