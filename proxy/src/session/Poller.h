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
    Vector<PollItem> items;
    Context context;
    int timeout_millisec;
};

#ifdef WJP_TEST
extern void test_router_connect();
extern void test_router();
extern void test_poller();
#endif

#endif //DATAACCESSPROXY_POLLER_H
