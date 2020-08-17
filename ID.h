#ifndef RISC_V_ID_H
#define RISC_V_ID_H

#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include "text_excute.h"
#include "Pipeline.h"

extern unsigned x[], locked[];
extern unsigned branch_address[][2], branch_vis_time[], branch_taken[][1 << 2], branch_history[];

extern const unsigned N;

class ID : public pipeline{
public:
    void getType(){
        opcode = get(ins, 6, 0);
        func3 = get(ins, 14, 12);
        switch (opcode){
            case 55: // 0110111
                type = U;
                break;
            case 23: // 0010111
                type = U;
                break;
            case 111: // 1101111
                type = J;
                break;
            case 103: // 1100111
                type = I;
                break;
            case 99: // 1100011
                type = B;
                break;
            case 3: // 0000011
                type = I;
                break;
            case 35: // 0100011
                type = S;
                break;
            case 19: // 0010011
                type = I;
                break;
            case 51: // 0110011
                type = R;
                break;
            default:
                break;
        }
    }

    void getOther(){
        switch (type){
            case R:
                func7 = get(ins, 31, 25);
                rs2 = get(ins, 24, 20);
                rs1 = get(ins, 19, 15);
                func3 = get(ins, 14, 12);
                rd = get(ins, 11, 7);
                break;
            case I:
                imm = get(ins, 31, 20);
                rs1 = get(ins, 19, 15);
                func3 = get(ins, 14, 12);
                rd = get(ins, 11, 7);
                break;
            case S:
                imm = get(ins, 31, 25) << 5 | get(ins, 11 ,7);
                rs2 = get(ins, 24, 20);
                rs1 = get(ins, 19, 15);
                func3 = get(ins, 14, 12);
                break;
            case B:
                imm = get(ins, 31, 31) << 12 | get(ins, 30, 25) << 5 | get(ins, 11, 8) << 1 | get(ins, 7, 7) << 11;
                rs2 = get(ins, 24, 20);
                rs1 = get(ins, 19, 15);
                func3 = get(ins, 14, 12);
                break;
            case U:
                imm = get(ins, 31, 12) << 12;
                rd = get(ins, 11, 7);
                break;
            case J:
                imm = get(ins, 31, 31) << 20 | get(ins, 30, 21) << 1 | get(ins, 20, 20) << 11 | get(ins, 19, 12) << 12;
                rd = get(ins, 11, 7);
                break;
            default:
                throw "error in getOther";
        }
    }

    void sign_extend(){
        switch (type){
            case R: // no imm
                break;
            case I: case S: case B:
                imm |= get01(get(ins, 31, 31), 31, 12);
                break;
            case U: // nothing to do
                break;
            case J:
                imm |= get01(get(ins, 31, 31), 31, 20);
                break;
            default:
                throw "error in sign extend";
        }
    }

    bool register_fetch(){
        switch (type){
            case R: case S: case B:
                if (locked[rs2] || locked[rs1]) return false;
                rs2 = x[rs2];
                rs1 = x[rs1];
                break;
            case I:
                if (locked[rs1]) return false;
                rs1 = x[rs1];
                break;
            case U: case J:
                break;
            default:
                throw "error in regiter fetch";
        }
        return true;
    }

    void lock_register(){
        switch (type){
            case R:	case I:	case U:	case J:
                locked[rd]++;
                break;
            default: // no rd
                break;
        }
    }

    void lock_pc(){
        if (opcode == 0b1101111 || opcode == 0b1100111) // JAL, JALR
            pc_lock++;
    }

    /* two-level adaptive predictor (better, based on automaton) */
    void dynamic_branch_predictor2(){
        if (opcode != 0b1100011) return;

        unsigned ins_address = ((pc - 4) >> 2) & (N-1);
        if (branch_vis_time[ins_address] == 0){ // init target address

            branch_address[ins_address][0] = pc;
            branch_address[ins_address][1] = pc - 4 + imm;
        }

        unsigned &tmp = branch_taken[ins_address][branch_history[ins_address] & MASK];
        if (tmp == 0) tmp = LEN >> 1 | 1; // init
        if (tmp <= LEN >> 1) // not taken
            pc = branch_address[ins_address][imm = 0]; // make choice
        else // taken
            pc = branch_address[ins_address][imm = 1]; // make choice

        rd = ins_address; // save the address into buffer
        branch_vis_time[ins_address]++;
    }

    void run(pipeline *next_ppl){
        if (!is_empty(next_ppl) || is_empty(this)) return;
        getType();
        getOther();
        sign_extend();
        if (!register_fetch()) return;  // hazard : unable to fetch the locked registers
        lock_register(); // hazard : lock the rd register
        lock_pc(); // hazard : lock pc


        dynamic_branch_predictor2();

        pass(next_ppl);
    }
};


#endif //RISC_V_ID_H
