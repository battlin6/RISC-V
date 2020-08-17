#include <cstdio>
#include <map>
#include "text_excute.h"
#include "Pipeline.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "MEM.h"
#include "WB.h"

unsigned mem[140001]; // memory
unsigned x[32], pc; // registers

unsigned locked[32], pc_lock; // hazard

const unsigned N = 1 << 13;

unsigned branch_address[N][2]; // the two addresses some branch may take, 0 for not taken, 1 for taken
unsigned branch_vis_time[N]; // the time some branch is visited
unsigned branch_history[N]; // the history of some branch, 0 for not taken, 1 for taken
unsigned branch_taken2[N][1 << 2]; // the state of the automaton of some branch

unsigned tot_num,right_num;

/* show the state of all pipelines */
int show_id;
void show_pipeline(int id, pipeline *ppl1, pipeline *ppl2, pipeline *ppl3, pipeline *ppl4, pipeline *ppl5){
    printf("%d : ", id);
    printf("%d %d %d %d %d\n", !ppl1->empty, !ppl2->empty, !ppl3->empty, !ppl4->empty, !ppl5->empty);
}

void Init(){
    unsigned pos = 0;
    while (true){
        char c = getc();
        if (c == EOF){
            break;
        }
        else if (c == '@'){
            unsigned v = 0;
            for (int i = 1; i <= 8; i++)
                v = v << 4 | hextrans(getc());
            pos = v;
        }
        else{
            unsigned v = hextrans(c) << 4 | hextrans(getc());
            mem[pos++] = v;
        }
    }
}

int main(){
    freopen("in", "r", stdin);

    Init();
    IF *ppl1 = new IF();
    ID *ppl2 = new ID();
    EX *ppl3 = new EX();
    MEM *ppl4 = new MEM();
    WB *ppl5 = new WB();

    while (true){
        ppl5->run(nullptr);
        //show_pipeline(++show_id, ppl1, ppl2, ppl3, ppl4, ppl5);
        ppl4->run(ppl5);
        //show_pipeline(++show_id, ppl1, ppl2, ppl3, ppl4, ppl5);
        ppl3->run(ppl4, ppl2);
        //show_pipeline(++show_id, ppl1, ppl2, ppl3, ppl4, ppl5);
        ppl2->run(ppl3);
        //show_pipeline(++show_id, ppl1, ppl2, ppl3, ppl4, ppl5);
        ppl1->run(ppl2);
        //show_pipeline(++show_id, ppl1, ppl2, ppl3, ppl4, ppl5);
        // if (!ppl3->empty)ppl3->show_buffer();
    }
}
