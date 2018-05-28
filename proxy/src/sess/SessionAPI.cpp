//
// Created by jipeng on 5/24/18.
//

#include "SessionAPI.h"
#include "zapi.h"
#include "ThreadPool.h"

void SessionAPI::start_server(int port, int poller_timeout, int nr_iothreads)
{
    auto callback=[](Message& addr, Message& content, RouterSocket& socket){
        std::cout<<content.str()<<std::endl;
        Message response=msg("Now I read: "+str(content));
        socket.sendToReq(addr, response);
    };
    RouterPoller poller{port, callback, poller_timeout, nr_iothreads};
    poller.start();
}