//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"

int main()
{
    auto callback=[](String addr, String content,  Context& context){
        zmq::socket_t requester(context, ZMQ_REQ);
        requester.connect("inproc://"+boost::lexical_cast<String>(INPROC_PORT));
        String inproc_ret=addr+content;
        s_send(requester, inproc_ret);
    };
    RouterPoller poller{5559, callback, -1, 10, 30};
    poller.start();
    return 0;
}