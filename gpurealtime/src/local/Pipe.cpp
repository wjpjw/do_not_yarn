//
// Created by jipeng on 5/8/18.
//

#include "Pipe.h"

Pipe::Pipe(const String& cmd)
{
    file = popen(cmd.c_str(), "r");
}

Pipe::~Pipe() {
    pclose(file);
}

String Pipe::out(){
    StringStream ss;
    char buf[1024];
    while(fgets(buf, 1024, file)){
        ss << buf;
    }
    return ss.str();
}