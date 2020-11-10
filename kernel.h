#ifndef KERNEL_H
#define KERNEL_H


#include "pcb.h"


typedef struct ReadyQueueNode {
    PCB*  aPCB;
    struct ReadyQueueNode* next;
} ReadyQueueNode;

extern ReadyQueueNode *head;
extern ReadyQueueNode *tail;

void scheduler();
void addToReady(PCB *pcb);

int boot();
int kernel();

#endif