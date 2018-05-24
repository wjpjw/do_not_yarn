//
// Created by jipeng on 5/22/18.
//

#include "Poller.h"
#include <memory>
#include "TestSocket.h"

Poller::Poller(int timeout_millisec):timeout_millisec(timeout_millisec), context(1)
{
    /* add acceptor socket */
    std::cout<<"poller ctor duhh!";
    add("*", CONTROL_PORT, [](const Message& message){
        std::cout<<message.str();
        char buf[message.size()+1];
        bzero(buf,message.size()+1);
        memcpy(buf,message.data(),message.size());
        std::cout<<"buf:"<<buf;
    });
    std::cout<<"poller ctor done!";
}


void Poller::add(const String& ip, int port, Callback callback){

//    new TestSocket<ZMQ_ROUTER>(context,ip,port,callback);
    sockets2.emplace_back(context, ip, port, callback);

//    sockets.emplace_back(context, ip, port, callback);
//    items.emplace_back(PollItem{(void*)sockets.back(), 0, ZMQ_POLLIN, 0});
}


/*
 *
 *  DEALER is like an asynchronous REQ socket, and ROUTER is like an asynchronous REP socket.
 *  Where we use a REQ socket, we can use a DEALER; we just have to read and write the envelope ourselves.
 *  Where we use a REP socket, we can stick a ROUTER; we just need to manage the identities ourselves.
 *
 * [1] Communication Pattern
 * N clients[REQi] : 1 proxy[Poller{ROUTER0,ROUTER1,ROUTER2...}]
 * From a client's perspective, its call is blocking and synchronized.
 * From the proxy's perspective, its event loop is always nonblocking.
 * Every socket in the poller must be nonblocking and asynchronized.
 * Thus they are ROUTER sockets.
 * These ROUTER sockets in theory could talk to multiple clients.
 * But they must not, except the first one, the Acceptor.
 *
 * [2] Acceptor
 * ZMQ does not include accept() in its design;
 * Acceptor is a trick to get around it.
 *
 * Acceptor:
 * 1. It can not be deleted
 * 2. Just like every socket in the poller, acceptor is also a ROUTER socket, nonblocking & async.
 *    On connected, it does reply politely that hey client you got connected.
 * 3. Every client must submit its ip:port info to apply for connnection.
 * 4. Then acceptor will help register a ZMQ_REQ socket in the poller.
 * 5. It binds a special port specified in the command line arg.
 * 6. On pollin, it should expect messages like this: "ip:port|ip:port|ip:port".
 *
 * */

void Poller::start(){
    while(true){
        zmq::poll (items.data(), items.size(), timeout_millisec);
        for(int i=0; i<sockets.size(); i++){
            auto& socket= sockets[i];
            if(items[i].revents & ZMQ_POLLIN){
                std::cout<<"pollin"<<std::endl;
                socket.onPollHit();
            }
            else{
                if(i!=0) socket.onPollMiss();  // Acceptor doesn't need a heartbeat protocol
            }
        }
    }
}
