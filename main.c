#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "myhistory.h"
#include "cd.c"

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

            //Delete leading whitespace
            for(int i = 0; i < strlen(cmd); ++i){
                    if(!isspace(cmd[i])){
                            strncpy(cmd, cmd + i, strlen(cmd));
                            break;
                    }
            }

            //Delete trailing whitespace
            for(int i = strlen(cmd) - 1; i >= 0 && isspace(cmd[i]); --i){
                   cmd[i] = '\0';
            }

            //Delete extra whitespace in the middle...
            bool currSpace = false;
            bool nextSpace = false;

            for(int i = 0; i < strlen(cmd); ++i){

                    currSpace = isspace(cmd[i]);
                    nextSpace = isspace(cmd[i+1]);

                    if(currSpace){
                        if(nextSpace){
                            for(int j = i; j < strlen(cmd); ++j){
                                    cmd[j] = cmd[j+1];
                            }
                            --i;
                        }
                    }
            }

            //If user input contains multiple commands, separate at ';' and execute sequentially

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
