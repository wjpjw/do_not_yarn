//
// Created by jipeng on 5/25/18.
//

#include "RouterPoller.h"
#include "RouterSocket.h"
#include "SimpleRouterSocket.h"

RouterPoller::RouterPoller(int port, Callback callback, int timeout_millisec, int nr_iothreads, int nr_workerthreads)
        :timeout_millisec(timeout_millisec),
         context(nr_iothreads),
         port(port),
         callback(callback),
         nr_workerthreads(nr_workerthreads)
{
}

void RouterPoller::start() {
    RouterSocket router_socket(context, port, callback, nr_workerthreads);
    SimpleRouterSocket async_response_socket(context, INPROC_PORT);

    PollItem items[] = {
            {router_socket,         0, ZMQ_POLLIN, 0},
            {async_response_socket, 0, ZMQ_POLLIN, 0}
    };
    while ( true ) {
        zmq::poll(&items[0], 2, timeout_millisec);
        if (items[0].revents & ZMQ_POLLIN) {
            router_socket.recvFromReq();
        }
        if (items[1].revents & ZMQ_POLLIN) {
            String addr, content;
            async_response_socket.recvFromReq(addr, content);
            std::cout<<"Now I respond to: "<<content<<std::endl;
            Message addr_msg=msg(addr);
            Message content_msg=msg(content);
            router_socket.sendToReq(addr_msg, content_msg);
        }
    }
}

