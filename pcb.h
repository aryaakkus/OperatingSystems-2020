#ifndef PCB_H
#define PCB_H

typedef struct PCB
{
    int pid;
    int PC_page;
    int PC_offset;
    int lines_max;
    int pages_max;
    int pageTable[10];
    int PC;
    
}PCB;

PCB* makePCB(int pid, int pages_max, int lines_max);

#endif

