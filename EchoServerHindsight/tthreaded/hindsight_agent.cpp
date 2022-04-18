#include <iostream>
#include <stdlib.h>
#include <unistd.h>

extern "C"{
    #include "tracer/agentapi.h"
    #include "tracer/hindsight.h"
    #include "tracer/buffer.h"
    #include "tracer/common.h"
}

HindsightAgentAPI* init_agentapi(const char* name){
    HindsightAgentAPI* api = hindsight_agentapi_init(name);

    std::cout<<"Initialized existing buffer manager "<<name<<std::endl;

    return api;

}

