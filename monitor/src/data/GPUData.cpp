//
// Created by jipeng on 5/14/18.
//

#include "GPUData.h"

GPU::GPU(const wjp::GPU& gpu) :power_percent(gpu.power_percent()), memory_percent(gpu.memory_percent()),
                            util_percent(gpu.util_percent()), bus_id(gpu.bus_id()), name(gpu.name())
{
}

GPUInfo::GPUInfo(const wjp::GPUs& gpus)
{
    for(int i=0;i<gpus.gpus_size();i++){
        map[gpus.gpus(i).bus_id()]=std::make_unique<GPU>(gpus.gpus(i));
    }
}
String GPUInfo::str()
{
    StringStream ss;
    for(auto& i : map){
        ss<<"  "<<i.first<<": "<<i.second->name<<", memory "<<i.second->memory_percent<<", util "
          <<i.second->util_percent<<", power "<<i.second->power_percent<<std::endl;
    }
    return ss.str();
}

void GPUData::setGPUInfo(const String& ip_port, const wjp::GPUs& gpus)
{
    map[ip_port]=std::make_unique<GPUInfo>(gpus);
}

const GPUInfo& GPUData::getGPUInfo(const String& ip_port)
{
    return *map[ip_port].get();
}
const GPU& GPUData::getGPU(const String& ip_port, const String& bus_id)
{
    return *map[ip_port]->map[bus_id].get();
}

String GPUData::str()
{
    StringStream ss;
    for(auto& i : map){
        ss<<i.first<<std::endl<<i.second->str()<<std::endl;
    }
    return ss.str();
}