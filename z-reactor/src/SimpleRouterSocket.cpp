//
// Created by jipeng on 5/28/18.
//

#include "SimpleRouterSocket.h"

SimpleRouterSocket::SimpleRouterSocket(Context& context, int port)
        :socket(context, ZMQ_ROUTER), port(port)
{
    socket.bind("inproc://"+boost::lexical_cast<String>(port));
}

void SimpleRouterSocket::recvFromReq(String& addr, String& content)
{
    Message message;
    socket.recv(&message);
    socket.recv(&message);
    socket.recv(&message);
    auto message_str=str(message); //message_str是addr和content拼起来的
    addr=message_str.substr(0,5);  //addr长度永远是5
    content=message_str.substr(5);
}

void SimpleRouterSocket::sendToReq(Message& addr, Message& content)
{
    socket.send(addr, ZMQ_SNDMORE);
    socket.send(Message{0}, ZMQ_SNDMORE);
    socket.send(content);
}