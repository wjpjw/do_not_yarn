//
// Created by jipeng on 5/25/18.
//

#include "RouterSocket.h"

#define MAXHP 4

RouterSocket::RouterSocket(Context& context, int port, Callback callback)
        :socket(context, ZMQ_ROUTER), port(port), callback(callback), hp(MAXHP)
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
    callback(addr_message, content_message, *this);
}

void RouterSocket::sendToReq(Message& addr, Message& content)
{
    socket.send(addr, ZMQ_SNDMORE);
    socket.send(Message{0}, ZMQ_SNDMORE);
    socket.send(content);
}