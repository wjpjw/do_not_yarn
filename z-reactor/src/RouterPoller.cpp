//
// Created by jipeng on 5/25/18.
//

#include "RouterPoller.h"

RouterPoller::RouterPoller(int port, Callback callback, int timeout_millisec, int nr_threads)
        :timeout_millisec(timeout_millisec),
         context(nr_threads),
         port(port),
         callback(callback)
{
}

void RouterPoller::start()
{
    RouterSocket socket(context,port,callback);
    PollItem items [] = {{ socket, 0, ZMQ_POLLIN, 0 }};
    while (true) {
        zmq::poll (&items [0], 1, timeout_millisec);
        if (items [0].revents & ZMQ_POLLIN) {
            socket.recvFromReq();
        }
    }
}