add_library(stoff2d_core
    animation.c
    glad.c
    particle.c
    shader.c
    sprite_renderer.c
    stbi_image.c
    stoff2d.c)

target_include_directories(stoff2d_core PRIVATE 
    ${CMAKE_SOURCE_DIR}/vendor/glad
    ${CMAKE_SOURCE_DIR}/vendor)

target_include_directories(stoff2d_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(stoff2d_core PRIVATE glfw cds)

target_link_libraries(stoff2d_core PUBLIC clm)
