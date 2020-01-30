#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct MEM
{
   char *value;
   char *variable;
};
struct MEM memory[1000000];

int var_index = 0;
int memstore(char* var, char* val){
  int exists = 0;
    // check if such var exist
    for (int i=0; i < var_index; i++){
        // check if the var has existed
        struct MEM temp=memory[i];
        temp.variable=memory[i].variable;
        temp.value=memory[i].value;
        if (strcmp(temp.variable, var)==0){
            memory[i].value = strdup(val);
            exists = 1;
            break;
        }
    }
    if (exists== 0 && var_index >= 1000000){
        printf("NOT ENOUGH MEMORY ON SHELL!\n");
        return 0; //not enough memory
    }
    if (exists == 0){
        // if not then create new variable with the name
        struct MEM newmem;
        newmem.variable= strdup(var);
        newmem.value= strdup(val);
        memory[var_index]=newmem;
        var_index++;
        return 0;
    }
    return 0;
}
char* memextract(char* var){
    for (int i=0; i < var_index; i++){
    
        //if there is a variable with the corresponding name
        struct MEM temp=memory[i];
        if (strcmp(temp.variable, var)==0){
            return temp.value;
        }
    }
    return "Variable not found!\n";}
