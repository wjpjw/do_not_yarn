//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ROUTERPOLLER_H
#define DO_NOT_YARN_ROUTERPOLLER_H

#include "zreactor.h"
#include "RouterSocket.h"

class RouterPoller {
public:
    RouterPoller(int port, Callback callback, int timeout_millisec=-1, int nr_threads=1);
    void start();
private:
    Context context;
    int timeout_millisec;
    Callback callback;
    int port;
};


#endif //DO_NOT_YARN_ROUTERPOLLER_H
