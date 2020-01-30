#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"

const int BUFSIZE = 128;
int recursiveCount=0;

//CORRESPONDING COMMANDS
int quit(){
printf("Bye!\n");
return -1;}

int help(){
    printf("COMMAND                 DESCRIPTION \n");
	printf("help                   Displays all the commands \n");
	printf("quit                   Exits / terminates the shell with Bye!\n");
	printf("set VAR STRING         Assigns a value to shell memory \n");
	printf("print VAR              Prints the STRING assigned to VAR \n");
	printf("run SCRIPT.TXT         Executes the file SCRIPT.TXT \n");
	return 0;}

int set(char *words[]){
    
    if(words[2]==NULL){printf("Error: Wrong Command Syntax.");
    return 0;}
    int i=3;
    char dest[1000];
    strcpy(dest,words[2]);
   
    while(words[i]!= NULL){
    strcat(dest," ");
    strcat(dest,words[i]);
    strcat(dest," ");
  
    i++;
    }
    char* value = strdup(dest);
    while(value[strlen(value)-1] == '\r' || value[strlen(value)-1] == '\n'){
			value[strlen(value)-1] = '\0';}
            
    return memstore(words[1],value);

}
int print(char *words[]){
if(words[1]==NULL){printf("Error: Wrong Command Syntax."); return 0;}
else{
    char *var = strdup(words[1]);
    char* val= memextract(var);
        printf("%s", val);
        printf("\n");
	    return 0;
    }
}
int run(char *word){
    

    
    int error=0;
    if(word==NULL){printf("Error: Wrong Command Syntax.\n"); return 0;}
    char * file= word;
    // printf("here1\n");

    while(file[strlen(file)-1] == '\r' || file[strlen(file)-1] == '\n'){
			file[strlen(file)-1] = '\0';}
    // printf("here2\n");
    
    char buffer[BUFSIZE];
    if (file==NULL){
        printf("Please enter a valid file name.\n");
        return 0;
    }
    else{
        
       
        FILE *fptr = fopen(file, "r");
        
        if(recursiveCount>128){
        printf("TERMINATED:This script has a an infinite loop!\n");
        
	    fclose(fptr);
        return 0;
        }

        if (fptr==NULL) {
         printf( "ERROR: Could not open textfile: %s\n",file);
         return 0;
        }
        recursiveCount++;
       
        
    
        while(fgets!=NULL){
		if(fgets(buffer, BUFSIZE, fptr) == NULL){
			printf("ERROR:Could not read the file or the end of the file is reached.\n");
           
	        fclose(fptr);
			return 0;
		}
		while(buffer[strlen(buffer)-1] == '\r' || buffer[strlen(buffer)-1] == '\n'){
			buffer[strlen(buffer)-1] = '\0';
		}
        
       
        error=parse(buffer);
        
        if(error==-1){
            fclose(fptr);
           _Exit(0);
        }
    }
     
    
	fclose(fptr);
	return error;
    }
}

int interpreter(char *words[]){
    //maps command to the corresponding function
    int errCode = 0;
    
    
  
    // The user is asking to execute a single command
    if ((strcmp(words[0], "quit"))==0) {return quit();}
    else if ((strcmp(words[0], "help"))==0) {return help();}
    else if (strcmp(words[0], "set")==0){return set(words);}
    else if (strcmp(words[0], "print")==0) {return print(words);}
    else if (strcmp(words[0], "run")==0) {return run(words[1]);}
    else {printf("Command unrecognized, please try again or type help to see the list of available commands.\n"); return 0; }
    return errCode;

    
}


