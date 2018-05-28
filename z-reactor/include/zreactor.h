//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ZREACTOR_H
#define DO_NOT_YARN_ZREACTOR_H

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

#define HEARTBEAT_INTERVAL 1000
#define INPROC_PORT 31458
#define INPROC_DELIM "\n"

class RouterSocket;

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
using PollItem      =   zmq::pollitem_t; // native pollitem_t
using Callback      =   std::function<String(String addr, String content)>;

using Functor       =   std::function<void(void)>;
using Mutex         =   std::mutex;


/* linux high resolution sleep */
static inline void linux_sleep_msecs(int msecs) {
    struct timespec t;
    t.tv_sec = msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
}

/*  linux current time */
static inline int64_t linux_clock() {
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static inline String str(Message& message)
{
    return std::string(static_cast<char*>(message.data()), message.size());
}

static inline Message msg(const String& messagestr)
{
    Message message(messagestr.size());
    memcpy (message.data(), messagestr.data(), messagestr.size());
    return message;
}

#endif //DO_NOT_YARN_ZREACTOR_H
