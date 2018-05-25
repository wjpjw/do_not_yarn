//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ROUTERSOCKET_H
#define DO_NOT_YARN_ROUTERSOCKET_H

#include "zreactor.h"

class RouterSocket {
public:
    inline operator void* () noexcept { return socket; }
    RouterSocket(Context& context, int port, Callback callback);
    void recvFromReq();
    void sendToReq(Message& addr, Message& content);
private:
    Callback    callback;                   /* callback on pollin, required */
    Socket      socket;
    int         port;
    int         hp;                         /* hp is a measure of socket health */
};

#endif //DO_NOT_YARN_ROUTERSOCKET_H
