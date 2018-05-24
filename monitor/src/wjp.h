//
// Created by jipeng on 5/9/18.
//

#ifndef GPUREALTIME_WJP_H
#define GPUREALTIME_WJP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <exception>
#include <functional>
#include <zmq.hpp>
#include <boost/lexical_cast.hpp>

#define WJP_TEST
#define VERSION "1.0-Alpha"
#define PROGRAM_NAME "GPU-Monitor"
#define HEARTBEAT_INTERVAL 1000

/* some camel casing alias */

template<typename T>
using Vector        =   std::vector<T>;
template<typename T, int N>
using Array         =   std::array<T, N>;

using String        =   std::string;
using StringStream  =   std::stringstream;
using FilePointer   =   FILE*;

using Message       =   zmq::message_t;
using Context       =   zmq::context_t;
using Socket        =   zmq::socket_t;
using PollItem      =   zmq::pollitem_t;
using Callback      =   std::function<void(const Message& message)>;

#endif //GPUREALTIME_WJP_H
