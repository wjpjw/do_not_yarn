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
#include <sys/time.h>
#include <zmq.hpp>

#define WJP_TEST
#define VERSION "1.0-Alpha"
#define PROGRAM_NAME "GPU-Realtime"
#define HEARTBEAT_INTERVAL 1000

/* some camel casing alias */

template<typename T>
using Vector        =   std::vector<T>;

using String        =   std::string;
using StringStream  =   std::stringstream;
using FilePointer   =   FILE*;

using Message       =   zmq::message_t;
using Context       =   zmq::context_t;
using Socket        =   zmq::socket_t;
using PollItem      =   zmq::pollitem_t;
using Callback      =   std::function<void(const Message& message)>;

/* linux high resolution sleep */
static inline void linux_sleep_msecs(int msecs)
{
    struct timespec t;
    t.tv_sec = msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
}

/*  linux current time */
static inline int64_t linux_clock(){
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

#endif //GPUREALTIME_WJP_H
