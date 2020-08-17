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
class IF : public pipeline{
public:
    static bool check_pc_lock(){
        return pc_lock == 0;
    }

    void run(pipeline *next_ppl){
        if (!is_empty(next_ppl)) return;
        if (!check_pc_lock()) return; // hazard : unable to get pc as it is locked
        ins = read(4);
        pass(next_ppl);
    }
};


#endif //RISC_V_IF_H
