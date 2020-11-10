#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "memorymanager.h"
#include "ram.h"
#include "kernel.h"
#include "pcb.h"


int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(PCB *p);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);
int countLines(FILE *f);
int launchHelper(PCB *pcb, FILE* f, int numPages);
int pageFault(PCB *pcb);
int checkVictim(PCB *p, int ind);
int deleteStorageFile(int pid);



ReadyQueueNode *head, *tail;
char *RAM[40];
int my_pid = 0;  // Keep track of PID values


int launcher(FILE *f1){
    int error  = 0;
    char* storageFile;

   //creating backing storage file
    char *filePath;
    char createCommand[100];
    strcpy(createCommand, "touch ");
    //creating filepath from PID
    char storePath[100] = "BackingStore/";
    char txt[100] = ".txt";
    char name[100];
    sprintf(name, "%d", my_pid);
    strcat(storePath, name);
    strcat(storePath, txt);
    filePath = strdup(storePath);

    strcat(createCommand, filePath);
	system(createCommand);

	//file pointer
	storageFile = strdup(filePath);

    FILE *f2 = fopen(storageFile, "w");
    //copying to file
    char buffer[1000];
    while(fgets(buffer, sizeof buffer, f1) != NULL){
    if(buffer[0] == '\0' ||buffer[0] == '\n' || buffer[0] == ' '){
        continue;
    }
    fputs(buffer, f2);
    }

    fclose(f2);
    fclose(f1);


    FILE *f =fopen(storageFile, "r");
    int numPages = countTotalPages(f);
    int numLines = countLines(f);
    

    if (numPages > 10){
        deleteStorageFile(my_pid);
        return -7;
    }

    PCB *pcb  = makePCB(my_pid, numPages, numLines);
    error  = launchHelper(pcb, f, numPages);
    pcb->PC = pcb->pageTable[0];
    addToReady(pcb);
    my_pid++;

    return error;
}

int launchHelper(PCB *pcb, FILE* f, int numPages){
    int error = 0;
    int findVictimFlag = 0;
    if(numPages<=1){
        int frameNumber = findFrame();
	    if(frameNumber == -1){
		frameNumber = findVictim(pcb);
		findVictimFlag = 1;
	}

        //update page table
        loadPage(0, f, frameNumber);
        updatePageTable(pcb, 0, frameNumber, findVictimFlag);
    }

    if(numPages>1){
        for(int i=0; i<2; i++){
            int frameNumber = findFrame();
            if(frameNumber == -1){
            frameNumber = findVictim(pcb);
            findVictimFlag = 1;
            }

            
             loadPage(i, f, frameNumber);
            updatePageTable(pcb, i, frameNumber, findVictimFlag);

        }
    }
    return error;

}

void loadPage(int pageNumber, FILE *f, int frameNumber){
    fseek(f, 0, SEEK_SET);	
    int count = 0;
    int offset = 0;
	int linesToLoad= pageNumber *4;

    while(!feof(f)){
		size_t cap = 0;
        char *l = NULL;
        getline(&l, &cap, f);
    	if((count >= linesToLoad) && (count < linesToLoad + 4)){
    		RAM[frameNumber * 4 + offset]=strdup(l);
    		offset++;
    	}
    	count++;
    }
	
}


int findFrame(){
    for (int i = 0; i<=36; i=i+4){
        if(RAM[i]==NULL && RAM[i+1]==NULL && RAM[i+2]==NULL && RAM[i+3]==NULL){
            return i/4; 
        }
    }
    return -1;
}

int findVictim(PCB *p){
    srand(time(NULL));
    int ind = (rand() % 40)/4;
    int flag=0;
    while(checkVictim(p, ind)){
        ind=(ind+1)%40;

    }
    return ind;
}

int checkVictim(PCB *p, int ind){
    size_t size  =sizeof(p->pageTable)/sizeof(int);
    for(int i=0; i<size; i++){
        if( p->pageTable[i]== ind){
            return 1;
        }
    }
    return 0;



}

int countTotalPages(FILE *f){
    int count  = countLines(f);
    int remainder = count % 4;
    int numPages  = (count/4) + remainder;

    return numPages;
}

int countLines(FILE *f){
    int count = 0;
    fseek(f,0, SEEK_SET);
    while(!feof(f)){
        size_t cap = 0;
        char *l = NULL;
        getline(&l, &cap, f);
        count++;

    }
    return count;
}



//duzeltt
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame){
    if(victimFrame !=0){
        ReadyQueueNode *ptr = head;
		while((ptr != NULL) && (ptr != tail->next)){
			PCB *pcb = ptr-> aPCB;
			for(int i = 0; i < 10; i++){
                if((pcb->pageTable[i]) == frameNumber){
                      pcb->pageTable[i] =- 1;
                }
            }
			ptr = ptr->next;
		}
	}
	p->pageTable[pageNumber] = frameNumber;
	return 0;
    
}


int pageFault(PCB *pcb){

    int error  = 0;

    int findVictimFlag = 0;

    if(pcb->pageTable[pcb->PC_page]== -1){
        int frameNumber = findFrame();
        if(frameNumber == -1){
            frameNumber  = findVictim(pcb);
            findVictimFlag = 1;
        }

        char *filePath;
        char storePath[100] = "BackingStore/";
        char txt[100] = ".txt";
        char name[100];
        sprintf(name, "%d", pcb->pid);
        strcat(storePath, name);
        strcat(storePath, txt);
        filePath = strdup(storePath);

        FILE *f =  fopen(filePath, "r");
        loadPage(pcb->PC_page, f, frameNumber);
        if(error != 0) {
            return error;
        }

        updatePageTable(pcb, pcb->PC_page, frameNumber, findVictimFlag);


    }


    pcb->PC =((pcb->pageTable[pcb->PC_page]) *4)+ pcb->PC_offset;
    return error;

}


int deleteStorageFile(int pid){
    char* filePath;
    char remove[100];
    strcpy(remove, "rm ");

    char storePath[100] = "BackingStore/";
    char txt[100] = ".txt";
    char name[100];
    sprintf(name, "%d", pid);
    strcat(storePath, name);
    strcat(storePath, txt);
    filePath = strdup(storePath);

    if(strcmp(filePath, "BackingStore/") !=0 ){
        strcat(remove, filePath);
        system(remove);
    }

    return 0;

}


     


