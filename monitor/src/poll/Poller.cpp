//
// Created by jipeng on 5/14/18.
//

#include "Poller.h"

Poller::Poller(int timeout_millisec, Vector<String> data_sources):timeout_millisec(timeout_millisec), context(1){
    for(auto& d : data_sources){
        Vector<String> lines;
        boost::split(lines, d, boost::is_any_of(":"));
        String ip=lines[0];
        int port=boost::lexical_cast<int>(lines[1]);
        add(ip, port, [ip,port](const Message& message){ // callback
            std::cout<<ip<<":"<<port<<" says: "<<message.str()<<std::endl;
            if(message.size()==0)return; // ignore heart beats
            // todo: update & publish gpu cluster info here
            std::cout<<"Do something!"<<std::endl;
        });
    }
    start();
}

void Poller::add(const String& ip, int port, Callback callback){
    sockets.emplace_back(context, ip, port, callback);
    items.emplace_back(PollItem{(void*)sockets.back(), 0, ZMQ_POLLIN, 0});
}

void Poller::start(){
    while(true){
        zmq::poll (items.data(), items.size(), timeout_millisec);
        for(int i=0; i<sockets.size(); i++){
            auto& socket= sockets[i];
            if(items[i].revents & ZMQ_POLLIN){
                socket.onPollHit();
            }
            else{
                socket.onPollMiss();
            }
        }
    }
}
