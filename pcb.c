#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

PCB* makePCB(int pid, int pages_max, int lines_max){
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    // BU KISMI ANLAMADIM
    pcb->PC = 0;
    pcb->pid = pid;
   // burasi

    pcb->PC_offset =0;
    pcb->PC_page=0;
    pcb->pages_max = pages_max;
    pcb->lines_max = lines_max;
    for(int i = 0; i < 10; i++) {
        pcb->pageTable[i] = - 1;
    }
    return pcb;
}
