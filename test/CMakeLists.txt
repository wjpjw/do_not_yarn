cmake_minimum_required(VERSION 3.7)
project(test)
set(CMAKE_CXX_STANDARD 14)

include_directories(../wjputil/include)

set(SOURCE_FILES main.cpp)
add_executable(dnytest ${SOURCE_FILES})
target_link_libraries(dnytest wjputil)
