//
// Created by jipeng on 5/22/18.
//

#include "Poller.h"
#include <memory>
#include "TestSocket.h"

Poller::Poller(int timeout_millisec):timeout_millisec(timeout_millisec), context(1)
{
    /* add acceptor socket */
//    add("*", CONTROL_PORT, [](const Message& message){
//        std::cout<<message.str();
//        char buf[message.size()+1];
//        bzero(buf,message.size()+1);
//        memcpy(buf,message.data(),message.size());
//        std::cout<<"\n  buf:"<<buf;
//    });
    add("localhost", 5556, [](const Message& message){
        std::cout<<message.str();
    });
}


void Poller::add(const String& ip, int port, Callback callback){
    sockets.emplace_back(context, ip, port, callback);
    items.emplace_back(PollItem{(void*)sockets.back(), 0, ZMQ_POLLIN, 0});
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
                std::cout<<"pollin!"<<std::endl;
                socket.onPollHit();
            }
            else{
                if(i!=0) socket.onPollMiss();  // Acceptor doesn't need a heartbeat protocol
            }
        }
    }
}


#ifdef WJP_TEST
void test_router_connect()
{

}

void test_poller()
{
    Context context(5);
    Vector<TestSocket<ZMQ_ROUTER>> sockets;
    sockets.emplace_back(context, "*", 5559, [](const Message& message){
        std::cout<<"calllback~~~"<<std::endl;
    });
    zmq::pollitem_t items [] = {
            { sockets[0], 0, ZMQ_POLLIN, 0 }
    };
    std::cout<<"start loop"<<std::endl;
    int ct=0;
    while (1) {
        zmq::message_t message;
        zmq::poll (&items [0], 1, -1);
        if (items [0].revents & ZMQ_POLLIN) {
            std::cout<<"event: "<< (++ct) <<" @" << Time{}.str() <<std::endl;
            auto addr=s_recv(sockets[0].socket);
            auto empty=s_recv(sockets[0].socket);
            auto content=s_recv(sockets[0].socket);
            std::cout<< addr ;
            sockets[0].callback(message);
            s_sendmore(sockets[0].socket, addr);
            s_sendmore(sockets[0].socket, "");
            s_send(sockets[0].socket, "Duh!"+content);
        }
    }
}

/*
    REQ端发送 DATA帧，ROUTER收到对应的REQ的ID帧+空帧+DATA帧
    ROUTER端发送 ID帧+空帧+DATA帧， 对应的REQ端收到DATA帧
    当然REQ套接字必须先发送请求，再接收消息。顺序不可乱。
 */
static void test_router_req(Context& context,Socket& router, const char*link)
{
    Socket identified0(context, ZMQ_REQ);
    identified0.setsockopt(ZMQ_IDENTITY, "A0", 2);
    identified0.connect(link);
    s_send (identified0, "ROUTER-REQ");
    auto address = s_recv(router);
    auto empty = s_recv(router);
    auto content = s_recv(router);
    std::cout<<"addr: "<<address<<std::endl<<empty<<std::endl<<"content: "<<content<<std::endl;
    s_sendmore(router, address);
    s_sendmore(router, "");
    s_send(router, "Server Reply");
    auto reqRec=  s_recv(identified0);
    std::cout<<reqRec<<std::endl;
}

void test_router(){
    Context context;
    Socket router(context, ZMQ_ROUTER);
    router.setsockopt(ZMQ_IDENTITY, "Router", 6);
    auto link="tcp://127.0.0.1:5005";
    router.bind(link);
    test_router_req(context, router, link);
}
#endif
