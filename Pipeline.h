#ifndef RISC_V_PIPELINE_H
#define RISC_V_PIPELINE_H

#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include "text_excute.h"

enum TYPE { R, I, S, B, U, J };
class pipeline {
public:

    //some values
    unsigned ins, rs1, rs2, rd, imm, func3, func7, opcode;
    bool empty;
    TYPE type;

    pipeline() : empty(true) {}

    ~pipeline() {}

    pipeline &operator=(const pipeline &other) {
        ins = other.ins;
        rs1 = other.rs1;
        rs2 = other.rs2;
        rd = other.rd;
        imm = other.imm;
        func3 = other.func3;
        func7 = other.func7;
        opcode = other.opcode;
        empty = other.empty;
        type = other.type;
        return *this;
    }

    //pass to next
    //next is empty
    void pass(pipeline *next_ppl) {
        if (next_ppl != nullptr) {
            *next_ppl = *this;
            next_ppl->empty = false;
        }
        empty = true;
    }

};

#endif //RISC_V_PIPELINE_H
