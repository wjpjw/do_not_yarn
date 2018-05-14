//
// Created by jipeng on 5/10/18.
//

#include "Pusher.h"
#include "../local/LocalAPI.h"
#include <boost/lexical_cast.hpp>
#include "gpuinfo.pb.h"
#ifdef WJP_TEST
#include <boost/timer/timer.hpp>
#endif

Pusher::Pusher(int port, int interval):interval(interval), port(port){
    start();
}

/*
 * Realtime performance evaluation:
 *
 * In the experiment there are 3 gpus, hence 3 gpuinfo structs.
 * It takes 0.02s to produce an up-to-date message about these gpus.
 *
 * The message is encoded in a compact binary format.
 * A messsage that wraps data of these 3 gpus is 95 bytes.
 *
 * In real production environment 3 gpus per container is a fairly reasonable config.
 * Each gpu server node could carry up to 16 gpus.
 *
 * Thus maximum latency is still fairly low,
 * far lower than, say 1 second, the default query interval time.
 *
 * */
zmq::message_t Pusher::createMessage() noexcept
{
#ifdef WJP_TEST
    boost::timer::auto_cpu_timer t;
#endif
    auto gpus = LocalAPI::getGPUs();
    wjp::GPUs msg_gpus;
    for (auto& gpu: gpus) {
        wjp::GPU *msg_gpuptr = msg_gpus.add_gpus();
        msg_gpuptr->set_name(std::move(gpu.name));
        msg_gpuptr->set_bus_id(std::move(gpu.bus_id));
        msg_gpuptr->set_memory_percent(gpu.memory_percent);
        msg_gpuptr->set_util_percent(gpu.util_percent);
        msg_gpuptr->set_power_percent(gpu.power_percent);
    }
    /* ByteSizeLong is the actual size after Serialization. */
    Vector<char> buff(msg_gpus.ByteSizeLong());  // 3 gpus: 95 Bytes
    msg_gpus.SerializeToArray(buff.data(), buff.size());
    zmq::message_t gpuinfo{buff.size()};
    memcpy(gpuinfo.data(), buff.data(), buff.size());
    return gpuinfo;
}

/* A simple impl of eventloop. */
void Pusher::start() {
    zmq::context_t context {1}; // nr threads for I/O
    zmq::socket_t socket {context, ZMQ_PUSH};
    socket.bind("tcp://*:"+boost::lexical_cast<String>(port));
    /* run at certain time */
    int64_t heartbeat_at = linux_clock () + HEARTBEAT_INTERVAL;
    int64_t querygpu_at = linux_clock() + interval*1000;
    while (true) {
        try{
            linux_sleep_msecs(200);         // event loop interval = 200 ms
            if (linux_clock () > heartbeat_at) {
                heartbeat_at = linux_clock () + HEARTBEAT_INTERVAL;
                socket.send(Message{0});    // heart beats have no payload
            }
            if( linux_clock() > querygpu_at){
                querygpu_at = linux_clock() + interval*1000;
                socket.send (createMessage());
            }
        }
        catch(zmq::error_t& e){
            std::cout << "zmq error: "<< e.what() << std::endl;
            break;
        }
    }
}
