//
// Created by jipeng on 5/22/18.
//

#ifndef DATAACCESSPROXY_AUTOSOCKET_H
#define DATAACCESSPROXY_AUTOSOCKET_H



#include "wjp.h"
#include "WJPTime.h"

/*
 * Smart, robust and stateful
 * it even has hp and heartbeat
 * it could be dead or alive
 * */
template <int TYPE, int MAXHP=3>
class AutoSocket{
public:
    /* Ctor:
     *  context only used to construct socket
     *  callback must be provided
     *  initial health is MAXHP, so it is alive on created
     *  socket will attempt to connect upon created
     *      (luckily since it's zmq connect, it won't fail even if the client side is unavailable)
     * */
    AutoSocket(AutoSocket&& s):socket(std::move(s.socket)),port(s.port),ip(s.ip),hp(s.hp),
                               lastCheckpoint(std::move(s.lastCheckpoint)),callback(s.callback)
    {
    }
    AutoSocket(Context& context, const String& ip, int port, Callback callback)
            :socket(context, TYPE), port(port), ip(ip), hp(MAXHP), lastCheckpoint(),
             callback(callback){
        if(ip=="*"){  // Acceptor
            socket.bind("tcp://*:"+boost::lexical_cast<String>(port));
        }
        else{  // proxy-client Conn
            socket.connect("tcp://"+ip+":"+boost::lexical_cast<String>(port));
        }
    }
    inline bool recv (Message* msg){
        return socket.recv(msg);
    }
    /* for zmq_poll use only!
     * (void*) AutoSocket -> (void*) zmq::socket_t -> actual void* handle to the socket object
     * */
    inline operator void* () noexcept {
        return socket;
    }
    /* Called on every poll hit:
     *      reset hp to MAXHP
     *      run callback function
     * */
    inline void onPollHit() {
        layOnHands();
        Message message;
        if(recv(&message)) callback(message);
    }
    /* Called on every poll miss:
     *       check time elapsed since last poll hit
     *       is it longer than HEARTBEAT_INTERVAL?
     * */
    inline void onPollMiss(){
        auto now=Time{};
        if(now-lastCheckpoint>HEARTBEAT_INTERVAL){
            onHeartbeatMiss();
            lastCheckpoint=now;
        }
    }
private:
    /* called on every heartbeat miss by onPollMiss() */
    inline void onHeartbeatMiss(){
        fading();
        //todo
    }
    /* health recovery */
    inline void layOnHands() noexcept {
        hp=MAXHP;
        lastCheckpoint=Time{}; // update lastCheckpoint to now
    }
    /* health dropping */
    inline void fading() noexcept{
        hp--;
        if(hp==0)hp=0;
    }
    Time        lastCheckpoint;             /* now-lastCheckpoint > HEARTBEAT_INTERVAL --> timeout!  */
    Callback    callback;                   /* callback on pollin, required */
    Socket      socket;
    int         port;
    String      ip;
    int         hp;                         /* hp is a measure of socket health */
};



#endif //DATAACCESSPROXY_AUTOSOCKET_H
