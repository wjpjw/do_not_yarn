//
// Created by jipeng on 5/10/18.
//

#include "LocalAPI.h"

Vector<GPU> LocalAPI::getGPUs()
{
    Pipe pipe{"nvidia-smi"};
    Parser parser{pipe.out()}; // rvalue ctor
    return parser.parseAll();
}