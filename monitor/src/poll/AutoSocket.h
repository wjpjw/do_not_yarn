//
// Created by jipeng on 5/14/18.
//

#ifndef DO_NOT_YARN_AUTOSOCKET_H
#define DO_NOT_YARN_AUTOSOCKET_H

#include "wjp.h"


template <int TYPE, int MAXHP=3>
class AutoSocket{
public:
    AutoSocket(Context& context, const String& ip, int port, Callback callback):
            socket(context, TYPE), port(port), ip(ip), hp(MAXHP), callback(callback){
        socket.connect("tcp://"+ip+":"+boost::lexical_cast<String>(port));
    }
    inline bool recv (Message* msg){
        return socket.recv(msg);
    }
    inline operator void* () noexcept {
        return socket;
    }
    inline void onPollHit() {
        hp=MAXHP;
        Message message;
        if(recv(&message)) callback(message);
    }
    inline void onPollMiss(){
        hp--;
        if(hp==0){
            //todo: tell k8s to restart gpurealtime @ip:port !
            std::cout<<"No heartbeat from gpurealtime @"<<ip<<":"<<port<<" for a while, probably dead."<<std::endl;
        }
    }
private:
    Socket      socket;
    int         port;
    String      ip;
    int         hp;
    Callback    callback;
};


#endif //DO_NOT_YARN_AUTOSOCKET_H
