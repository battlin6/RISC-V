#ifndef RISC_V_TEXT_EXCUTE_H
#define RISC_V_TEXT_EXCUTE_H

extern unsigned mem[];
extern unsigned pc;

unsigned hextrans(char v){
    if ('0' <= v && v <= '9') return v - '0';
    else if ('a' <= v && v <= 'z') return v - 'a' + 10;
    else return v - 'A' + 10;
}

unsigned get01(unsigned v, int high, int low){
    if (!v) return 0;
    else return ((1 << (high - low + 1)) - 1) << low;
}

unsigned get(unsigned ins, int high, int low){
    return ins << (31 - high) >> (32 - (high - low + 1));
}

char getc(){
    char c = getchar();
    while (c == ' ' || c == '\n')
        c = getchar();
    return c;
}

unsigned read(int len){
    unsigned v = 0;
    for (int i = len - 1; ~i; i--)
        v = v << 8 | mem[pc + i];
    pc += len;
    return v;
}

unsigned read(int pos, int len){
    unsigned v = 0;
    for (int i = len - 1; ~i; i--)
        v = v << 8 | mem[pos + i];
    return v;
}

void write(int pos, int len, int v){
    for (int i = 0; i < len; i++){
        mem[pos + i] = v & ((1 << 8) - 1);
        v >>= 8;
    }
}

#endif //RISC_V_TEXT_EXCUTE_H
