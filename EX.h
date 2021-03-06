#ifndef RISC_V_EX_H
#define RISC_V_EX_H


#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include "text_excute.h"
#include "Pipeline.h"

extern unsigned tot_num,right_num;

class EX : public pipeline{
public:
    void execute(pipeline *prev_ppl){
        switch (opcode){
            case 0b0110111: // LUI
                break;
            case 0b0010111: // AUIPC
                break;
            case 0b1101111: // JAL
                imm -= 4;
                break;
            case 0b1100111: // JALR
                imm += rs1;
                (imm >>= 1) <<= 1;
                break;
            case 0b1100011: // ...
                // imm -= 4;
                switch (func3){
                    case 0b000: // BEQ
                        rs1 = rs1 == rs2 ? 1 : 0;
                        break;
                    case 0b001: // BNE
                        rs1 = rs1 != rs2 ? 1 : 0;
                        break;
                    case 0b100: // BLT
                        rs1 = int(rs1) < int(rs2) ? 1 : 0;
                        break;
                    case 0b101: // BGE
                        rs1 = int(rs1) >= int(rs2) ? 1 : 0;
                        break;
                    case 0b110: // BLTU
                        rs1 = rs1 < rs2 ? 1 : 0;
                        break;
                    case 0b111: // BGEU
                        rs1 = rs1 >= rs2 ? 1 : 0;
                        break;
                }

                tot_num++;

                (branch_history[rd] <<= 1) |= rs1;

                if (rs1 != imm){ // incorrect prediction
                    prev_ppl->empty = true;
                    pc_lock++;
                }
                else{ // correct prediction
                    right_num++;
                }
                break;
            case 0b0000011: // ...
                imm += rs1;
                switch (func3){
                    case 0b000: // LB
                        break;
                    case 0b001: // LH
                        break;
                    case 0b010: // LW
                        break;
                    case 0b100: // LBU
                        break;
                    case 0b101: // LHU
                        break;
                }
                break;
            case 0b0100011: // ...
                imm += rs1;
                switch (func3){
                    case 0b000: // SB
                        break;
                    case 0b001: // SH
                        break;
                    case 0b010: // SW
                        break;
                }
                break;
            case 0b0010011: // ...
                switch (func3){
                    case 0b000: // ADDI
                        rs1 += imm;
                        break;
                    case 0b010: // SLTI
                        rs1 = int(rs1) < int(imm) ? 1 : 0;
                        break;
                    case 0b011: // SLTIU
                        rs1 = rs1 < imm ? 1 : 0;
                        break;
                    case 0b100: // XORI
                        rs1 = rs1 ^ imm;
                        break;
                    case 0b110: // ORI
                        rs1 = rs1 | imm;
                        break;
                    case 0b111: // ANDI
                        rs1 = rs1 & imm;
                        break;
                    case 0b001: // SLLI
                        rs1 <<= (imm & ((1 << 5) - 1));
                        break;
                    case 0b101: // ...
                        switch (func7){
                            case 0b0000000: // SRLI
                                rs1 >>= (imm & ((1 << 5) - 1));
                                break;
                            case 0b0100000: // SRAI
                                rs1 = int(rs1) >> (imm & ((1 << 5) - 1));
                                break;
                        }
                        break;
                }
                break;
            case 0b0110011: // ...
                switch (func3){
                    case 0b000: // ...
                        switch (func7){
                            case 0b0000000: // ADD
                                rs1 = rs1 + rs2;
                                break;
                            case 0b0100000: // SUB
                                rs1 = rs1 - rs2;
                                break;
                        }
                        break;
                    case 0b001: // SLL
                        rs1 <<= rs2;
                        break;
                    case 0b010: // SLT
                        rs1 = int(rs1) < int(rs2) ? 1 : 0;
                        break;
                    case 0b011: // SLTU
                        rs1 = rs1 < rs2 ? 1 : 0;
                        break;
                    case 0b100: // XOR
                        rs1 = rs1 ^ rs2;
                        break;
                    case 0b101: // ...
                        switch (func7){
                            case 0b0000000: // SRL
                                rs1 >>= rs2;
                                break;
                            case 0b0100000: // SRA
                                rs1 = int(rs1) >> rs2;
                                break;
                        }
                        break;
                    case 0b110: // OR
                        rs1 = rs1 | rs2;
                        break;
                    case 0b111: // AND
                        rs1 = rs1 & rs2;
                        break;
                }
                break;
        }
    }

    void make_triple_cycle(){
        if (opcode == 0b0000011 || opcode == 0b0100011) // L**, S**
            func7 = 3;
    }

    void run(pipeline *next_ppl, pipeline *prev_ppl){
        if (!is_empty(next_ppl) || is_empty(this)) return;
        execute(prev_ppl);
        make_triple_cycle();
        pass(next_ppl);
    }
};

#endif //RISC_V_EX_H
