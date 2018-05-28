//
// Created by jipeng on 5/28/18.
//

#ifndef DO_NOT_YARN_SIMPLEROUTERSOCKET_H
#define DO_NOT_YARN_SIMPLEROUTERSOCKET_H

#include "zreactor.h"

class SimpleRouterSocket {
public:
    inline operator void* () noexcept { return socket; }
    SimpleRouterSocket(Context& context, int port);
    void recvFromReq(String& addr, String& content);
    void sendToReq(Message& addr, Message& content);
private:
    Socket      socket;
    int         port;
};


#endif //DO_NOT_YARN_SIMPLEROUTERSOCKET_H
