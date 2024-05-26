add_library(cds
    src/cds_exlist.c
    src/cds_utils.c)

target_include_directories(cds PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
