aux_source_directory(. DIR_WORLD_SRCS)
add_library(poll ${DIR_WORLD_SRCS} Poller.cpp Poller.h AutoSocket.h)
