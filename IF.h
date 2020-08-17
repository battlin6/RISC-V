#ifndef RISC_V_IF_H
#define RISC_V_IF_H


#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include "text_excute.h"
#include "Pipeline.h"

extern unsigned pc_lock;
class pipeline1 : public pipeline
{
public:
    bool test_pc_permission()
    {
        if (pc_lock)
            return false;
        else
            return true;
    }
    void run(pipeline *next_ppl)
    {
        if (!is_empty(next_ppl)) return;
        if (!test_pc_permission()) return; // hazard : unable to get pc as it is locked
        ins = read_mem32();
        pass(next_ppl);
    }
};


#endif //RISC_V_IF_H
