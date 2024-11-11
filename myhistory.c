/* Name: Corinna Martin
 * Section: 004
 * Date: 10/25/24
 * Description: myhistory lists the shell history of previously run commands
 *              myhistory -c clears the history
 *              myhistory -e <myhistory_number> executes one of 20 stored commands 
 */

#include "myhistory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void myhistory_add(char *newCmd);

char cmd_history[20][512]; //Array storing a max of 20 previously run commands
int history_num_cmds = 0; //Represents the # commands stored in history

//Lists the shell history of previously run commands
void myhistory(){

        printf("\n\t--- COMMAND HISTORY ---\n\n");

        //Print each command
        for(int i = 0; i < history_num_cmds; ++i){
                printf("\t#%d: %s\n", i+1, cmd_history[i]);
        }

        printf("\n\t-----------------------\n\n");
}

//Clears the history
void myhistory_c(){

        //Reset strings in history array
        for(int i = 0; i < 20; ++i){
                memset(cmd_history[i], 0, sizeof(cmd_history[i]));
                //cmd_history[i][0] = '\0';
        }

        //Reset # commands in array to zero
        history_num_cmds = 0;
}

//Executes one of 20 stored commands
void myhistory_e(int num){

        char numChar[2];

        //Execute specified command

        //Check for custom commands
        if(strcmp(cmd_history[num - 1], "myhistory") == 0) {myhistory();}
        else{
                //Execute other commands
                pid_t pid = fork();

                if(pid > 0){wait(NULL);}
                if(pid == 0){
                    execl("/bin/sh", "/bin/sh", "-c", cmd_history[num-1], (char *)0);
                    printf("\n");
                }

                //Add command to history
                if(strcmp(cmd_history[num-1], "\0") != 0){
                        myhistory_add(cmd_history[num-1]);
                }
        }
}

//Updates command history
void myhistory_add(char *newCmd){

        //Overwrite old commands when history array is full
        if(history_num_cmds == 20){

                --history_num_cmds;

                for(int i = 0; i < 19; ++i){
                        strcpy(cmd_history[i], cmd_history[i+1]);
                }
        }

        //Add most recent command to history array & update # commands
        strcpy(cmd_history[history_num_cmds], newCmd);
        ++history_num_cmds;
}
