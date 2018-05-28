//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"

int main()
{
    auto callback=[](String addr, String content){
        return addr+content;
    };
    RouterPoller poller{5559, callback, -1, 10, 30};
    poller.start();
    return 0;
}