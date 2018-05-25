//
// Created by jipeng on 5/22/18.
//

#ifndef DATAACCESSPROXY_WJP_H
#define DATAACCESSPROXY_WJP_H

/* Common Headers */
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <memory>
#include <exception>
#include <zmq.hpp>
#include <unordered_map>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>
#include <boost/algorithm/string.hpp>
#include <mutex>
#include <sys/time.h>
#include <time.h>
#include "zhelper.h"
/* Some Program Level Constants */
#define WJP_TEST
#define VERSION "1.0-Alpha"
#define PROGRAM_NAME "Data Access Client"
#define HEARTBEAT_INTERVAL 1000     /* clients are supposed to send at least an empty message every 1s */
#define CONTROL_PORT 12345
#define DATA_PORT   5555

/* Camel Casing Types */
template<typename T>
using Vector        =   std::vector<T>;
template<typename T, int N>
using Array         =   std::array<T, N>;
template <typename K, typename V>
using Map           =   std::unordered_map<K, V>;

using String        =   std::string;
using StringStream  =   std::stringstream;
using FilePointer   =   FILE*;

using Message       =   zmq::message_t;
using Context       =   zmq::context_t;
using Socket        =   zmq::socket_t;
using PollItem      =   zmq::pollitem_t;
using Callback      =   std::function<void(const Message& message)>;

using Functor       =   std::function<void(void)>;
using Mutex         =   std::mutex;


/* Some Inline Functions */

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

#endif //DATAACCESSPROXY_WJP_H
