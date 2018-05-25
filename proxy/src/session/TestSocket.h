//
// Created by jipeng on 5/23/18.
//

#ifndef DATAACCESSPROXY_TESTSOCKET_H
#define DATAACCESSPROXY_TESTSOCKET_H


#include "wjp.h"
template <int TYPE, int MAXHP=3>
class TestSocket{
public:
    TestSocket(Context& context, const String& ip, int port, Callback callback)
            : socket(context, TYPE), port(port), ip(ip), hp(MAXHP),
             callback(callback){
        socket.bind("tcp://*:"+boost::lexical_cast<String>(port));
    }
    inline void onPollHit() {

    }
    inline void onPollMiss(){
    }
    inline operator void* () noexcept {
        return socket;
    }
    Callback    callback;                   /* callback on pollin, required */
    Socket      socket;
private:
    int         port;
    String      ip;
    int         hp;                         /* hp is a measure of socket health */
};




#endif //DATAACCESSPROXY_TESTSOCKET_H
