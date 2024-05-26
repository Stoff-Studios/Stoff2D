add_library(stoff2d_ecs
    component_map.c
    stoff2d_ecs.c)

target_include_directories(stoff2d_ecs PUBLIC 
    ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(stoff2d_ecs PUBLIC clm)
target_link_libraries(stoff2d_ecs PRIVATE cds)
