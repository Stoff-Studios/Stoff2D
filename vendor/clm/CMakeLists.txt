add_library(clm
    src/clm.c)

target_include_directories(clm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
