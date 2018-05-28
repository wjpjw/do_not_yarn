include_directories(/Users/wjp/CLionProjects/z-reactor/include)
aux_source_directory(. DIR_SESS_SRCS)
add_library(sess ${DIR_SESS_SRCS})
