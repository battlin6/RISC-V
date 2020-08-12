#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include "text_excute.h"
#include "memory_excute.h"
#include "Pipeline.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "MEM.h"
#include "WB.h"

int x[33],pc; //register
int mem[131072+7]; //memory

int lock[32],pc_lock; //hazards todo

int main() {
    freopen("","r",stdin);


    //init
    Init();

    IF *p1 = new IF;
    ID *p2 = new ID;
    EX *p3 = new EX;
    MEM *p4= new MEM;
    WB *p5= new WB;


    //excute

    while(true){
        p5->run(nullptr);
        p4->run(p5);
        p3->run(p4,p2);
        p2->run(p3);
        p1->run(p2);
    }

}
