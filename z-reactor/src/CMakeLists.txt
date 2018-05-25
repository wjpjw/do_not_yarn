# boost settings
set(BOOST_ROOT "/opt/local/include/boost")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66.0 COMPONENTS regex timer program_options)
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(../include)

if(Boost_FOUND)
    aux_source_directory(. DIRSRCS)
    add_library(zreactor SHARED ${DIRSRCS})
    target_link_libraries(zreactor ${Boost_LIBRARIES} zmq)
endif()

