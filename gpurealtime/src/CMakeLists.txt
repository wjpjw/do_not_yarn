# boost settings
set(BOOST_ROOT "/opt/local/include/boost")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66.0 COMPONENTS regex timer program_options)
include_directories(/opt/local/include)
link_directories(/opt/local/lib)
include_directories(${Boost_INCLUDE_DIRS})

if(Boost_FOUND)
    aux_source_directory(. DIRSRCS)
    include_directories(. local world)
    add_subdirectory(local)
    add_subdirectory(world)
    add_executable(gpurealtime ${DIRSRCS})
    target_link_libraries(gpurealtime ${Boost_LIBRARIES} local world zmq protobuf)
endif()

