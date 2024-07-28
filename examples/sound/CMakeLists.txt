add_executable(audio
    src/audio.c
    )

target_link_libraries(audio PRIVATE stoff2d_audio miniaudio)

