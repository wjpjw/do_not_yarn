aux_source_directory(. DIR_WORLD_SRCS)
add_library(data ${DIR_WORLD_SRCS} GPUData.cpp GPUData.h)
