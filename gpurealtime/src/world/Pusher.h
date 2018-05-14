//
// Created by jipeng on 5/10/18.
//

#ifndef GPUREALTIME_REPLIER_H
#define GPUREALTIME_REPLIER_H

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

/*
 * Pusher Version 1: simple req-rep pattern
 *
 * todo list:
 *
 * 1. Client-side reliability design: monitor should retry several times.
 *
 * 2. One-way heartbeat: If gpurealtime dies, monitor should be informed. If monitor dies, it probably has a good reason to die anyway.
 *
 * */

class Pusher {
public:
    Pusher(const Pusher&) = delete;
    void operator=(const Pusher&)= delete;
    Pusher(int port, int interval);
private:
    void start();
    zmq::message_t createMessage() noexcept;
    int interval;
    int port;
};


#endif //GPUREALTIME_REPLIER_H
