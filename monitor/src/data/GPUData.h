//
// Created by jipeng on 5/14/18.
//

#ifndef DO_NOT_YARN_GPUDATA_H
#define DO_NOT_YARN_GPUDATA_H

#include "wjp.h"
#include <unordered_map>
#include <memory>
#include <boost/algorithm/string.hpp>
#include "poll/gpuinfo.pb.h"

template <typename K, typename V>
using Map = std::unordered_map<K,V>;

struct GPU{
    GPU(const wjp::GPU& gpu);
    String      name;               // Tesla K80
    String      bus_id;             // 0000:14:00.0
    int         util_percent;       // might not actually reflect availability
    int         memory_percent;     // probably the most important measure
    int         power_percent;      // pwr/cap = power_percent %
};

struct GPUInfo{
    GPUInfo(const wjp::GPUs& gpus);
    Map<String, std::unique_ptr<GPU>> map;
    String str();
};

struct DataSourceID{
    String ip;
    int port;
    inline String str(){
        StringStream ss;
        ss<<ip<<":"<<port;
        return ss.str();
    }
};

static inline DataSourceID parse_ip_port(const String& ipport){
    Vector<String> lines;
    boost::split(lines, ipport, boost::is_any_of(":"));
    String ip=lines[0];
    int port=boost::lexical_cast<int>(lines[1]);
    return {ip,port};
}

class GPUData {
public:
    const GPU& getGPU(const String& ip_port, const String& bus_id);
    const GPUInfo& getGPUInfo(const String&);
    void setGPUInfo(const String& ip_port, const wjp::GPUs& gpus);
    String str();
private:
    Map<String, std::unique_ptr<GPUInfo>> map;
};

#endif //DO_NOT_YARN_GPUDATA_H
