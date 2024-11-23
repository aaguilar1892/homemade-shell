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
#include "alias.h"
#include "path.h"
#include "pipelining.h"
#include "exit.h"

void runCommands(char cmd[512], char num[2]);

int main(int argc, char* argv[]){

    // Initialize path at the start of the program
    initialize_path();

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

            if (cmd[strlen(cmd)-1] != '\n') {
                printf("Input too long, couldn't execute\n");
                while (cmd[strlen(cmd)-1] != '\n') {
                    fgets(cmd, 512, stdin);
                }
                continue;
            }
            
            cmd[strlen(cmd)-1] = '\0';


            //Seperate user input into individual commands (seperated by ';' if applicable)
            int cc=0;//currcmd char index iterator
            for (int c = 0; c<strlen(cmd)+1 /*include null terminator*/; c++) {
                currCmd[cc] = cmd[c];//copy char crom cmd to currCmd 
                if (cmd[c] == ';' || cmd[c] == '\0') {
                    currCmd[cc] = '\0';//null terminate the currCmd string
                    //execute current command
                    runCommands(currCmd, num);
                    //clear currCmd for next command
                    cc=0;
                }
                else {
                    //set currCmd char to cmd char
                    currCmd[cc] = cmd[c];
                    cc++;//iterate to next char space
                }
            }

        }
    }

    //Batch mode
    if(argc == 2){
        FILE *input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "Error opening input file, make sure that the file exists and try again\n");
            return -1;
        }
        printf("Batch mode, each line echoed in bold, function outputs (if any) follow\n");
        while(fgets(cmd,512,input_file)) {  
            printf("\e[1m%s\e[m",cmd);  
            if (cmd[strlen(cmd)-1] != '\n') {
                printf("\e[1mInput too long, couldn't execute\e[m\n");
                while (cmd[strlen(cmd)-1] != '\n') {
                    fgets(cmd, 512, input_file);
                }
                continue;
            }

            //Seperate input into individual commands (seperated by ';' if applicable)
            int cc=0;//currcmd char index iterator
            for (int c = 0; c<strlen(cmd)+1 /*include null terminator*/; c++) {
                currCmd[cc] = cmd[c];//copy char crom cmd to currCmd 
                if (cmd[c] == ';' || cmd[c] == '\0') {
                    currCmd[cc] = '\0';//null terminate the currCmd string
                    //execute current command
                    runCommands(currCmd, num);
                    //clear currCmd for next command
                    cc=0;
                }
                else {
                    //set currCmd char to cmd char
                    currCmd[cc] = cmd[c];
                    cc++;//iterate to next char space
                }
            }

        }
        //close input file stream
        fclose(input_file);
    }

    //Print error message if incorrect # of command line arguments
    if(argc != 1 && argc != 2){
            fprintf(stderr, "\n\t --- ERROR: Incorrect number of command line arguments ---\n\n");
            fprintf(stderr, "\tInteractive Mode: newshell\n");
            fprintf(stderr, "\tBatch Mode: newshell [batchFile]\n\n");
            exit(1);
    }

    restore_path();
    return 0;
}

void runCommands(char cmd[512], char num[2]) {
 
    //Remove extra white space from command
    strcpy(cmd, preprocessing(cmd));

    //If empty command line, process next command
    if(strcmp(cmd, "\0") == 0 || strncmp(cmd, " ", 1) == 0 || strcmp(cmd, ";") == 0){
        return;
    }

    // Check for pipeline command first
    if (strchr(cmd, '|') != NULL) {
        handle_pipeline(cmd);
        myhistory_add(cmd);
        return;
    }

    //Check for I/O redirection
    char fileName[512];
    char *ltPtr = strchr(cmd, '<');
    char *gtPtr = strchr(cmd, '>');

    if(ltPtr){
        int i = (int)(ltPtr - cmd);
        strcpy(fileName, cmd + i+2);

        input_redirection(cmd, fileName);
        myhistory_add(cmd);
        return;
    }
    if(gtPtr){
        int i = (int)(gtPtr - cmd);
        strcpy(fileName, cmd + i+2);

        output_redirection(cmd, fileName);
        myhistory_add(cmd);
        return;
    }

    // Handle path command
    if (strncmp(cmd, "path", 4) == 0) {
        char *args[4];  // Maximum 3 arguments plus NULL
        int arg_count = 0;
        char *token = strtok(cmd, " ");
        
        while (token != NULL && arg_count < 3) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        
        path_builtin(args);
        myhistory_add(cmd);
        return;
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
        char alName[503]; //char array for input (length enough for max name and command length in form name[100]='command[400]') 
        //if no input, print existing aliases
        if ((strcmp(cmd, "alias")==0)||(strcmp(cmd,"alias;")==0)) {
            printAl();
            myhistory_add(cmd);
        }
        //if -c flag set, clear all aliases
        else if ((strcmp(cmd, "alias -c")==0)||(strcmp(cmd,"alias -c;") == 0)) {
            aliasClear();
            myhistory_add(cmd);
        }
        //if -r flag set, remove specific alias
        else if (strncmp(cmd,"alias -r ",9)==0) {
            
            strcpy(alName, cmd+9);
            aliasDel(alName);
            myhistory_add(cmd);
        }
        //add alias
        else if (strncmp(cmd, "alias ", 6) == 0) {
            
            strcpy(alName, cmd+6);
            aliasAdd(alName);
            myhistory_add(cmd);
        }
    }
    //cd command
    else if(strncmp(cmd, "cd",2)==0) {
        if ((strcmp(cmd,"cd")==0)||(strcmp(cmd,"cd;")==0)) {
            cd(NULL);
            myhistory_add(cmd);
        }
        else {
            char path[512];
            strcpy(path, cmd+3);
            cd(path);
            myhistory_add(cmd);
        }
    }
	//exit command
	else if(strncmp(cmd, "exit", 4)==0){
		if(strcmp(cmd, "exit")==0){
			exit_builtin();
		}
	}
    else{
        //Execute other commands
        pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Error: Failed to create process: %s\n", strerror(errno));
        return;
    }

    if (pid == 0) {  // Child process
        // Try to execute the command directly first
        char *args[512];
        int arg_count = 0;
        
        // Parse command into arguments
        char *token = strtok(cmd, " ");
        while (token != NULL && arg_count < 511) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        execvp(args[0], args);

        // If execvp fails, try using shell
        execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
        
        // If we get here, both execution attempts failed
        fprintf(stderr, "Error: Command '%s' not found\n", args[0]);
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        // Add command to history if execution was successful
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            myhistory_add(cmd);
        }
    }
}
}
