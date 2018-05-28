//
// Created by jipeng on 5/25/18.
//

#include "RouterSocket.h"

RouterSocket::RouterSocket(Context& context, int port, Callback callback, int nr_threads)
        :socket(context, ZMQ_ROUTER), port(port),
         callback(callback), threadpool(nr_threads), context(context)
{
    socket.bind("tcp://*:"+boost::lexical_cast<String>(port));
}

/*
    req->router的消息封装如下：
    地址，空帧，内容
    消息分为三段。
 */
void RouterSocket::recvFromReq()
{
    Message addr_message;
    socket.recv(&addr_message);
    Message content_message;
    socket.recv(&content_message);
    socket.recv(&content_message);
    //todo performance test
    String addr=str(addr_message);
    String content=str(content_message);
    //这里必须以值语义捕获addr与content，复制到线程栈中。用户发过来的都是很小的请求报文，复制一下问题不大。
    threadpool.execute<Functor>([this, addr, content](){
        Socket requester(context, ZMQ_REQ);
        requester.connect("inproc://"+boost::lexical_cast<String>(INPROC_PORT));
        s_send(requester, callback(addr, content));
    });
}

void RouterSocket::sendToReq(Message& addr, Message& content)
{
    socket.send(addr, ZMQ_SNDMORE);
    socket.send(Message{0}, ZMQ_SNDMORE);
    socket.send(content);
}