#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "myhistory.h"

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

            //If user input contains multiple commands, separate at ';' and execute sequentially

        }
    }

    //Batch mode
    if(argc == 2){

    }

    //Print error message if incorrect # of command line arguments
    if(argc != 1 && argc != 2){
            fprintf(stderr, "\n\t --- ERROR: Incorrect number of command line arguments ---\n\n");
            fprintf(stderr, "\tInteractive Mode: ./a.out\n");
            fprintf(stderr, "\tBatch Mode: ./a.out <batch_file>\n\n");
            exit(1);
    }

    return 0;
}
