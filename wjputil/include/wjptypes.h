//
// Created by jipeng on 5/15/18.
//

#ifndef DO_NOT_YARN_WJPTYPES_H
#define DO_NOT_YARN_WJPTYPES_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <exception>
#include <zmq.hpp>
#include <unordered_map>
#include <boost/lexical_cast.hpp>


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

#endif //DO_NOT_YARN_WJPTYPES_H
