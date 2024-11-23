/*Name: Elisabeth Nguyen
 *Command: Alias
 *Description:  printAl() prints a list of the currently stored aliases
 *              getAlias() takes a string input of the alias name and returns the index that the alias pair is stored at, returns -1 if alias doesn't exist
 *              checkAlias() checks if input is an alias name, if yes, replaces the string with the corresponding command
 *              aliasAdd() if alias name not in use, adds the name/command pair to the alias database
 *              aliasDel() if alias name exists, removes the name/command pair from the database
 *              aliasClear() removes all aliases from database (sets numAliases to 0)
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"
//Alias database
#define MAXALIAS 50 //the maximum number of aliases that can be stored

char aliasNames[MAXALIAS][100];//array storing the alias names
char aliasCmds[MAXALIAS][400];//array storing the corresponding commands
int numAliases = 0; 

void printAl() {
    if (numAliases == 0) {
        printf("Currently no aliases defined. Add an alias using the format: alias name='command'\n");
    }
    else {
        for (int i=0; i<numAliases; i++) {
        printf("%d. %s = %s\n", (i+1), aliasNames[i], aliasCmds[i]);
        }
    }
    
}

int getAlias(char* name) {
    for (int i=0; i<numAliases; i++) {
        if (strcmp(aliasNames[i],name)==0) {
            return i;
        }
        
    }
    return -1;
}

void checkAlias (char* name) {
    int index = getAlias(name);
    if (index != -1) {
        strcpy(name, aliasCmds[index]);
    }
}

void aliasAdd(char* input) {
    char* alias;
    char* command;
    alias = strtok(input,"='");
    if (alias == NULL) {
        fprintf(stderr, "alias could not be added, check format (name='command')\n");
        return;
    }

    command = strtok(NULL, "'");
    if (command == NULL) {
        fprintf(stderr, "alias could not be added, check format (name='command')\n");
        return;
    }


    //see if the alias already exists
    int index = getAlias(alias);
    if (index == -1) {
        strcpy(aliasNames[numAliases], alias);
        strcpy(aliasCmds[numAliases], command);
        numAliases +=1;
        printf("alias added: %s = '%s'\n", alias, command);
    }
    else {
        printf("Alias name already exists: Use alias command without input to see existing aliases.");
    }
}

void aliasDel(char* name) {
    int index = getAlias(name);
    if (index == -1) {
        printf("Alias does not exist\n");
    }
    else {
        for(int i=index; i<numAliases; i++) {
            strcpy(aliasNames[i], aliasNames[i+1]);
            strcpy(aliasCmds[i], aliasCmds[i+1]);
        }
        numAliases--;
    }
}
void aliasClear() {
    //set numAliases to 0, values will still remain, but will be inaccessable and overwritten when new alias is added
    numAliases = 0;
}
