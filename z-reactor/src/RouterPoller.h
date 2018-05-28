//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ROUTERPOLLER_H
#define DO_NOT_YARN_ROUTERPOLLER_H

#include "zreactor.h"

class RouterPoller {
public:
    RouterPoller(int port, Callback callback, int timeout_millisec=-1, int nr_iothreads=1, int nr_workerthreads=10);
    void start();
private:
    int nr_workerthreads;
    Context context;
    int timeout_millisec;
    Callback callback;
    int port;
};


#endif //DO_NOT_YARN_ROUTERPOLLER_H
