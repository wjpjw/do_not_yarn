//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"

int main()
{
    auto callback=[](Message& addr, Message& content, RouterSocket& socket){
        std::cout<<content.str()<<std::endl;
        Message response=msg("Now I read: "+str(content));
        socket.sendToReq(addr, response);
    };
    RouterPoller poller{5559, callback};
    poller.start();
    return 0;
}