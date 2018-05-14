//
// Created by jipeng on 5/8/18.
//

#include "Parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>

Parser::Parser(String&& pipeout) : gpuinfo(std::move(pipeout)){}

Parser::Parser(const String& pipeout) : gpuinfo(pipeout){}

void Parser::printAll(const Vector<GPU>& gpus) const
{
    std::cout<<"\nPrint All GPUs:\n";
    for(auto&i:gpus){
        std::cout<<"  "<<i.name<<"\t bus id: "<<i.bus_id
                 <<"\t util percent: "<<i.util_percent<<"\%"
                 <<"\t memory percent: "<<i.memory_percent<<"\%"
                 <<"\t power percent: "<<i.power_percent<<"\%\n";
    }
}

Vector<GPU> Parser::parseAll()const
{
    Vector<GPU> gpus;
    Vector<String> lines;
    boost::split(lines, gpuinfo, boost::is_any_of("\n"));
    if(lines.size()<16){ // smi output at least
        throw std::runtime_error("nr of lines < expected");
    }
    for(int i=7;i<lines.size();i+=3){ // starts from row 7
        if(lines[i][0]==' '){
            break;
        }
        gpus.emplace_back(parseGPU(lines[i],lines[i+1]));
    }
    // printAll(gpus);
    return gpus;
}

GPU Parser::parseGPU(const String& line1, const String& line2)const
{
    GPU gpu;

    // line 1
    Vector<String> segs;
    boost::split(segs, line1, boost::is_any_of("|"));

    // line 1: name
    Vector<String> words;
    boost::split_regex(words, segs[1], boost::regex("\\s\\s+"));
    gpu.name=std::move(words[2]);

    Vector<String> words_busid;
    boost::split_regex(words_busid, segs[2], boost::regex("\\s\\s+"));

    // line 1: bus_id
    gpu.bus_id=std::move(words_busid[0]);
    boost::trim(gpu.bus_id);

    // line 2
    Vector<String> segs2;
    boost::split(segs2, line2, boost::is_any_of("|"));

    // line 2: power_percent
    Vector<String> words_power;
    boost::split_regex(words_power, segs2[1], boost::regex("\\s\\s+"));

    // line 2: power_percent: power/cap
    String powerstr=words_power[words_power.size()-1];
    powerstr=boost::erase_all_copy(powerstr, "W");

    Vector<String> pcap;
    boost::split(pcap, powerstr, boost::is_any_of("/"));

    boost::trim(pcap[0]);
    boost::trim(pcap[1]);

    int power = boost::lexical_cast<int>(pcap[0]);
    int cap = boost::lexical_cast<int>(pcap[1]);
    gpu.power_percent= (int) (100*power/cap);

    // line 2: memory_percent
    String memstr=segs2[2];
    memstr=boost::erase_all_copy(memstr, "MiB");
    Vector<String> mcap;
    boost::split(mcap, memstr, boost::is_any_of("/"));

    boost::trim(mcap[0]);
    boost::trim(mcap[1]);

    int mem = boost::lexical_cast<int>(mcap[0]);
    int memcap = boost::lexical_cast<int>(mcap[1]);
    gpu.memory_percent= (int) (100*mem/memcap);

    // line 2: util_percent
    Vector<String> words_util;
    boost::split_regex(words_util, segs2[3], boost::regex("\\s\\s+"));
    String utilstr=words_util[1];
    utilstr=boost::erase_all_copy(utilstr, "\%");
    boost::trim(utilstr);
    gpu.util_percent= boost::lexical_cast<int>(utilstr);

    return gpu;
}
