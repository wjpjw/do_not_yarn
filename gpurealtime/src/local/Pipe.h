//
// Created by jipeng on 5/8/18.
//

#ifndef GPUREALTIME_PIPE_H
#define GPUREALTIME_PIPE_H

#include "../wjp.h"

class Pipe {
public:
    // explicitly ban copy operator & ctor
    Pipe(const Pipe&)=delete;
    void operator=(const Pipe&)=delete;
    // RAII, popen a command
    Pipe(const String& cmd);
    // clean up: close pipe
    virtual ~Pipe();
    // stdout of the command; add 2>&1 to include stderr
    String out();
private:
    FilePointer file;
};


#endif //GPUREALTIME_PIPE_H
