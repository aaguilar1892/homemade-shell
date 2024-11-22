#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include "myhistory.h"
#include "preprocessing.h"
#include "cd.h"
#include "redirecting.h"

int main(int argc, char* argv[]){

    char cmd[512]; //Represents user-entered string
    char currCmd[512]; //Represents individual command to be processed
    char num[2]; //Represents # command to execute from history

    //Interactive mode
    if(argc == 1){

        //Until user exits shell
        while(1 == 1){
            
            //Get user input
            printf("Enter command: ");
            fgets(cmd, 512, stdin);
            cmd[strlen(cmd)-1] = '\0';

            //Remove extra white space from command
            strcpy(cmd, preprocessing(cmd));

            //If empty command line, process next command
            if(strcmp(cmd, "\0") == 0 || strncmp(cmd, " ", 1) == 0 || strcmp(cmd, ";") == 0){
                    continue;
            }

            //If user input contains multiple commands, separate at ';' and execute sequentially

            //Check for I/O redirection
            char fileName[512];
            char *ltPtr = strchr(cmd, '<');
            char *gtPtr = strchr(cmd, '>');

            if(ltPtr){
                int i = (int)(ltPtr - cmd);
                strcpy(fileName, cmd + i+2);

                input_redirection(cmd, fileName);
                continue;
            }
            if(gtPtr){
                int i = (int)(gtPtr - cmd);
                strcpy(fileName, cmd + i+2);

                output_redirection(cmd, fileName);
                continue;
            }

            //Check if command is an alias
            checkAlias(cmd);
            //Execute custom commands
            if(strncmp(cmd, "myhistory", 9) == 0){
                //Execute myhistory commands
                if(strcmp(cmd, "myhistory") == 0 || strcmp(cmd, "myhistory;") == 0){
                        myhistory_add(cmd);
                        myhistory();
                }
                if(strncmp(cmd, "myhistory -e", 12) == 0 && isdigit(cmd[13])){
                        strncpy(num, cmd + 13, strlen(cmd));
                        myhistory_e(atoi(num));
                }
                if(strcmp(cmd, "myhistory -c") == 0 || strcmp(cmd, "myhistory -c;") == 0){
                        myhistory_add(cmd);
                        myhistory_c();
                }

                //Usage statements
                if(strncmp(cmd, "myhistory -e", 12) == 0 && !isdigit(cmd[13])) {printf("Usage: myhistory -e <myhistory_number>\n");}
            }
            //Alias command
            else if(strncmp(cmd, "alias", 5) == 0) {
                //test
                printf("reached alias\n");

                char alName[503]; //char array for input (length enough for max name and command length in form name[100]='command[400]') 
                //if no input, print existing aliases
                if ((strcmp(cmd, "alias")==0)||(strcmp(cmd,"alias;")==0)) {
                    //test
                    printf("printing aliases\n");
                    printAl();
                }
                //if -c flag set, clear all aliases
                else if ((strcmp(cmd, "alias -c")==0)||(strcmp(cmd,"alias -c;") == 0)) {
                    //test 
                    printf("alias clearing (not remove)\n");
                    aliasClear();
                }
                //if -r flag set, remove specific alias
                else if (strncmp(cmd,"alias -r ",9)==0) {
                    //test
                    printf("removing alias\n");
                    
                    strcpy(alName, cmd+9);
                    printf("%s\n", alName);
                    aliasDel(alName);

                }
                //add alias
                else if (strncmp(cmd, "alias ", 6) == 0) {
                    //test
                    printf("adding alias\n");
                    
                    strcpy(alName, cmd+6);
                    printf("%s\n", alName);
                    aliasAdd(alName);
                }
            }
            //cd command
            else if(strncmp(cmd, "cd",2)==0) {
                if ((strcmp(cmd,"cd")==0)||(strcmp(cmd,"cd;")==0)) {
                    cd(NULL);
                }
                else {
                    char path[512];
                    strcpy(path, cmd+3);
                    printf("%s\n",path);
                    cd(path);
                }
            }
            else{
                //Execute other commands
                pid_t pid = fork();

                if(pid > 0){wait(NULL);}
                if(pid == 0){
                    execl("/bin/sh", "/bin/sh", "-c", cmd, (char *)0);
                    printf("\n");
                }
                
                //Add command to history
                myhistory_add(cmd);
            }
        }
    }

    //Batch mode
    if(argc == 2){

    }

    //Print error message if incorrect # of command line arguments
    if(argc != 1 && argc != 2){
            fprintf(stderr, "\n\t --- ERROR: Incorrect number of command line arguments ---\n\n");
            fprintf(stderr, "\tInteractive Mode: newshell\n");
            fprintf(stderr, "\tBatch Mode: newshell [batchFile]\n\n");
            exit(1);
    }

    return 0;
}
