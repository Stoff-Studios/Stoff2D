add_library(stoff2d_audio
    src/stoff2d_audio.c)

target_include_directories(stoff2d_audio PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(stoff2d_audio PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(stoff2d_audio PRIVATE miniaudio)
