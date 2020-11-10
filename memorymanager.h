#include "pcb.h"

#include <stdio.h>

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

int launcher(FILE *f1);
int deleteStorageFile(int pid);
int pageFault(PCB *pcb);


#endif /* MEMORYMANAGER_H_ */