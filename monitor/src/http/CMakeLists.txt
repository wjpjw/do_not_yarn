aux_source_directory(. DIR_WORLD_SRCS)
add_library(http ${DIR_WORLD_SRCS} HttpServer.cpp HttpServer.h)
