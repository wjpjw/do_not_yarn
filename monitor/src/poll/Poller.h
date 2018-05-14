//
// Created by jipeng on 5/14/18.
//

#ifndef DO_NOT_YARN_POLLER_H
#define DO_NOT_YARN_POLLER_H

#include "wjp.h"
#include "AutoSocket.h"
#include <boost/algorithm/string.hpp>
#include <memory>
#include "data/GPUData.h"
/* event loop starts on construction */
class Poller{
public:
    Poller(int timeout_millisec, Vector<String> data_sources);
private:
    void add(const String& ip, int port, Callback callback);
    void start();
    Vector<AutoSocket<ZMQ_PULL>> sockets;
    Vector<PollItem> items;
    Context context;
    int timeout_millisec;
    std::unique_ptr<GPUData> data;
};


#endif //DO_NOT_YARN_POLLER_H
