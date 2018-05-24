//
// Created by jipeng on 5/22/18.
//

#ifndef DATAACCESSPROXY_POLLER_H
#define DATAACCESSPROXY_POLLER_H

#include "wjp.h"
#include "AutoSocket.h"
#include "TestSocket.h"
class Poller{
public:
    Poller(int timeout_millisec);
    void add(const String& ip, int port, Callback callback);
    void start();
private:
    Vector<AutoSocket<ZMQ_ROUTER>> sockets;
    Vector<TestSocket<ZMQ_ROUTER>> sockets2;

    Vector<PollItem> items;
    Context context;
    int timeout_millisec;
};


#endif //DATAACCESSPROXY_POLLER_H
