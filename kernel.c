
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ram.h"
#include "cpu.h"
#include "kernel.h"
#include "shell.h"
#include "memorymanager.h"



void resetRAM();
void removeFrame(int ind);

ReadyQueueNode* head = NULL;
ReadyQueueNode* tail = NULL;
ReadyQueueNode* oldhead = NULL;
int sizeOfQueue = 0;

char *RAM[40];
CPU* cpu;


int main(int argc, char const *argv[]){
    int error  = 0;
    boot();
    error  = kernel();
    return error;
}


int kernel()
{
    shellUI();
}

int boot(){
    resetRAM();

    
    system("rm -f BackingStore");
    system("mkdir BackingStore");
    

    return 0;
}




void scheduler(){
    cpu = malloc(sizeof(CPU));
    cpu->quanta = 2;

    while((head != NULL) && (head != tail->next)){
        int interruptStatusFlag = 0;

        PCB* removeHead = head->aPCB;
        if(removeHead->pageTable[removeHead->PC_page] == -1) pageFault(removeHead);

        
        cpu->IP = removeHead->pageTable[removeHead->PC_page];	    
        cpu->offset = removeHead->PC_offset;

        oldhead = head;
        int InstructionsToExecute = removeHead->lines_max - ((removeHead->PC_page) * 4 + removeHead->PC_offset);
        
        
        if(cpu->quanta < InstructionsToExecute){
        	interruptStatusFlag = run(cpu->quanta);
        	if(interruptStatusFlag == 1){
        		removeHead->PC_offset = 0;
        		(removeHead->PC_page)++;
        		if(pageFault(removeHead) == 0) addToReady(removeHead); // Add PCB back to the end of the ready queue if resolved
        	}
        	else{
        		removeHead->PC_offset = cpu->offset;
        		removeHead->PC = (cpu->IP) * 4 + cpu->offset;
                addToReady(removeHead);		
        	}
        }
        
        else{
        	interruptStatusFlag = run(InstructionsToExecute);
            for(int i = 0; i < 10; i++){
            	if(removeHead->pageTable[i] != -1){
            		int ind = (removeHead->pageTable[i]) * 4;
                    removeFrame(ind);
            	}
            }
            deleteStorageFile(removeHead->pid);
            free(oldhead);
        }
        head = head->next;
    }
    head = NULL;
    tail = head;
    resetRAM(); 
    
       
}

void addToReady(PCB* pcb) {
    ReadyQueueNode* newNode = malloc(sizeof(ReadyQueueNode));
    newNode->aPCB = pcb;
    newNode->next = NULL;
    if (head == NULL){
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
        tail->aPCB = pcb;
    }
   
}


void resetRAM(){
    
    for(int i = 0; i < 40; i++) {
        RAM[i] = NULL;
    }
}




void removeFrame(int ind){

    for (int i = 0; i<4; i++){
        RAM[ind+i] = NULL;
    }

}