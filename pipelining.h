/*
Name : Alexis Aguilar
Description : This file contains the function prototypes and structures for the pipelining.c file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PIPE_COMMANDS 3
#define MAX_ARGS 50

// Structure to hold a parsed command
typedef struct {
    char *args[MAX_ARGS];
    int arg_count;
} Command;

void parse_command(char *cmd_str, Command *cmd);
void free_command(Command *cmd);
int parse_pipeline(char *input, Command commands[MAX_PIPE_COMMANDS]);
int execute_pipeline(Command commands[], int cmd_count);
int handle_pipeline(char *input);

/* TODO: Add function to main shell loop to check for pipeline commands
// Usage in main shell loop
int check_and_execute_pipeline(char *input) {
    // Check if input contains pipe symbol
    if (strchr(input, '|') != NULL) {
        return handle_pipeline(input);
    }
    return 0;  // Not a pipeline command
}
*/