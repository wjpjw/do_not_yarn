//
// Created by jipeng on 5/8/18.
//

#ifndef GPUREALTIME_PARSER_H
#define GPUREALTIME_PARSER_H

#include "../wjp.h"
#include <boost/algorithm/string.hpp>

/* GPU info */
struct GPU{
    String      name;               // Tesla K80
    String      bus_id;             // 0000:14:00.0
    int         util_percent;       // might not actually reflect availability
    int         memory_percent;     // probably the most important measure
    int         power_percent;      // pwr/cap = power_percent %
};


/* Parser of nvidia-smi output */
class Parser {
public:
    // transfer ownership: original pipeout obsolete (value swapped with gpuinfo)
    Parser(String&& pipeout);
    // ctor: copy pipeout, original one still alive
    Parser(const String& pipeout);
    // parse all gpu info
    Vector<GPU> parseAll()const;
private:
    void printAll(const Vector<GPU>&)const;
    GPU parseGPU(const String& line1, const String& line2)const;
    String gpuinfo;
};


#endif //GPUREALTIME_PARSER_H
