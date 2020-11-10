#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"ram.h"
#include"interpreter.h"
#include"shell.h"
#include "cpu.h"

#define DEFAULT_QUANTA 2
const int MAX_OFFSET=4;



int run(int quanta){
   
    
        while(quanta > 0){
        
    	int progaddress = (cpu->IP) * MAX_OFFSET +(cpu->offset);
        strcpy(cpu->IR, RAM[progaddress]);
        parse(cpu->IR);
        
        cpu->offset++;
        if(cpu->offset >= MAX_OFFSET) return 1;
        quanta--;
    }
    return 0;
}
