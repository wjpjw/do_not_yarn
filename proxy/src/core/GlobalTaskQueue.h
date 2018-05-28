//
// Created by jipeng on 5/22/18.
//

#ifndef DATAACCESSPROXY_GLOBALTASKQUEUE_H
#define DATAACCESSPROXY_GLOBALTASKQUEUE_H

#include "ConcurrentBlockingQueue.h"
#include "DataAccessTask.h"

class GlobalTaskQueue {
public:
    GlobalTaskQueue();
private:
    ConcurrentBlockingQueue<DataAccessTask> queue;
};


#endif //DATAACCESSPROXY_GLOBALTASKQUEUE_H
