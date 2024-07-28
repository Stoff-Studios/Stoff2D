add_library(miniaudio
    src/miniaudio.c
    )

target_include_directories(miniaudio PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
