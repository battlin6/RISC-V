#ifndef RISC_V_TEXT_EXCUTE_H
#define RISC_V_TEXT_EXCUTE_H

extern int mem[];

//16 to 10
int Hextrans(const char x) {
    if (x <= '9' && x >= '0')
        return x - '0';
    else if (x <= 'z' && x >= 'a')
        return x - 'a' + 10;
    else
        return x - 'A' + 10;
}

//just get char
char Getchar(){
    char x=getchar();
    while(x=='\n'||x==' '){
        x=getchar();
    }
    return x;
}

//init the memory
void Init() {
    int pos = 0;
    while (true) {
        char c = Getchar();
        if (c == EOF) {
            break;
        } else if (c == '@') {
            int v = 0;
            //get this 8-byte number
            for (int i = 1; i <= 8; i++)
                v = v << 4 | Hextrans(Getchar());
            pos = v;
        } else {
            //get two number
            int v = Hextrans(c) << 4 | Hextrans(Getchar());
            mem[pos++] = v;
        }
    }
}

void Mem_write(const int& pos,const int &len,int v) {
    for (int i = 0; i < len; ++i) {
        mem[pos + i] = v & ((1 << 8) - 1);
        v >>= 8;
    }
}

#endif //RISC_V_TEXT_EXCUTE_H
