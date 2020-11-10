#ifndef CPU_H
#define CPU_H
#define DEFAULT_QUANTA 2

int run(int);

typedef struct CPU{ 
    int IP; 
    char IR[1000]; 
    int quanta; 
    int offset;
} CPU;


extern CPU *cpu;

extern const int MAX_OFFSET;

#endif 