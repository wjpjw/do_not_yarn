include_directories(../include)
include_directories(../src)
link_directories(../src)
add_executable(zreactor-example main.cpp)
target_link_libraries(zreactor-example zreactor)

