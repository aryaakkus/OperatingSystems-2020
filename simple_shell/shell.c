#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <readline/readline.h>
// #include <unistd.h>
// #include <sys/wait.h>
#include <sys/types.h> 
#include "interpreter.h"
#include "shellmemory.h"

// Clearing the shell using escape sequences 

#define MAXCOM 1000 // max number of letters to be supported 


int parse(char input[]) {
    char tmp[200]={ NULL }; 
    int a = 0;
    int b = 0;
    char *words[1000]={ NULL }; 
    int w = 0; // wordsIdx

    //Skipping white spaces
    for (a = 0; input[a] == ' ' && a < 1000; a++)
        ;

    while (input[a] != '\0' && input[a] != '\n' && input[a] != '\r' && a < 1000)
    {

        for (b = 0; input[a] != '\0' && input[a] != '\n' && input[a] != '\r' && input[a] != ' ' && a < 1000; a++, b++)
            tmp[b] = input[a]; // extract a word

        tmp[b] = '\0';
        words[w] = strdup(tmp);
        a++;
        w++;
    }
    // assumes: cmd switches args
    return interpreter(words);
}
    




int main(int argc, char **argv){
    char inputString[MAXCOM]; 
    int errorCode= 0;
    //  clear();
    printf("Welcome to the AA shell.\nVersion 1.0 \nCreated in January 2020\n");
    while(1){
        printf("$ ");
        if(fgets(inputString, MAXCOM-1, stdin)==NULL){
            printf("Please enter a valid input.\n");
			return 0;}
        while(inputString[strlen(inputString)-1] == '\r' || inputString[strlen(inputString)-1] == '\n' || inputString[strlen(inputString)-1] == '\a'){
			inputString[strlen(inputString)-1] = '\0';}
        
        errorCode=parse(inputString);
        if (errorCode == -1) {_Exit(0);}



        
    }
    return 0;
}                            