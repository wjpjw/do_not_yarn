//
// Created by jipeng on 5/14/18.
//

#include "Poller.h"
#include "gpuinfo.pb.h"

Poller::Poller(int timeout_millisec, Vector<String> data_sources)
        :timeout_millisec(timeout_millisec), context(1), data(std::make_unique<GPUData>())
{
    for(auto& data_source : data_sources){
        auto id=parse_ip_port(data_source);
        add(id.ip, id.port, [data_source, this](const Message& message){
            /* callback code*/
            std::cout<< data_source <<" says: "<<message.str()<<std::endl;
            if(message.size()==0)return; // ignore heart beats
            wjp::GPUs msg_gpus;
            if(msg_gpus.ParseFromArray(message.data(), message.size())){
                data->setGPUInfo(data_source, msg_gpus);
                // todo: publish data
            }
            else{
                throw std::runtime_error("Cannot parse protobuf message!");
            }

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
            std::cout<<data->str()<<std::endl;
        }
    }
}
