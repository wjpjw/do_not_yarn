//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ROUTERSOCKET_H
#define DO_NOT_YARN_ROUTERSOCKET_H

#include "zreactor.h"
#include "ThreadPool.h"

class RouterSocket {
public:
    inline operator void* () noexcept { return socket; }
    RouterSocket(Context& context, int port, Callback callback, int nr_threads);
    void recvFromReq();
    void sendToReq(Message& addr, Message& content);
private:
    Callback    callback;                   /* callback on pollin, required */
    Socket      socket;
    int         port;
    ThreadPool  threadpool;
    Context&    context;
};

#endif //DO_NOT_YARN_ROUTERSOCKET_H
