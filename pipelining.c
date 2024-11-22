/*
Name : Alexis Aguilar
Description : This file contains the function implementations for the pipelining.h file. The functions are used to parse and execute a pipeline of commands.
*/

#include "pipelining.h"

// Function to split a command string into arguments
void parse_command(char *cmd_str, Command *cmd) {
    char *token;
    cmd->arg_count = 0;
    
    // Split command into arguments
    token = strtok(cmd_str, " \t");
    while (token != NULL && cmd->arg_count < MAX_ARGS - 1) {
        cmd->args[cmd->arg_count++] = strdup(token);
        token = strtok(NULL, " \t");
    }
    cmd->args[cmd->arg_count] = NULL;
}

// Function to free command structure
void free_command(Command *cmd) {
    for (int i = 0; i < cmd->arg_count; i++) {
        free(cmd->args[i]);
    }
}

// Function to parse input into multiple commands separated by pipes
int parse_pipeline(char *input, Command commands[MAX_PIPE_COMMANDS]) {
    char *cmd_str;
    int cmd_count = 0;
    
    // Split input by pipe symbol
    cmd_str = strtok(input, "|");
    while (cmd_str != NULL && cmd_count < MAX_PIPE_COMMANDS) {
        // Remove leading/trailing spaces
        while (*cmd_str == ' ') cmd_str++;
        
        // Parse individual command
        parse_command(cmd_str, &commands[cmd_count]);
        cmd_count++;
        
        // Get next command
        cmd_str = strtok(NULL, "|");
    }
    
    return cmd_count;
}

// Function to execute a pipeline of commands
int execute_pipeline(Command commands[], int cmd_count) {
    int pipes[2][2];  // Array to hold pipe file descriptors
    pid_t pids[MAX_PIPE_COMMANDS];
    
    // Create necessary pipes
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return -1;
        }
    }
    
    // Execute each command in the pipeline
    for (int i = 0; i < cmd_count; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            return -1;
        }
        
        if (pids[i] == 0) {  // Child process
            // Set up stdin for all except first command
            if (i > 0) {
                if (dup2(pipes[i-1][0], STDIN_FILENO) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            
            // Set up stdout for all except last command
            if (i < cmd_count - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            
            // Close all pipe file descriptors
            for (int j = 0; j < cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // Execute command
            execvp(commands[i].args[0], commands[i].args);
            
            // If execvp returns, there was an error
            fprintf(stderr, "Error: Failed to execute command '%s'\n", commands[i].args[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    // Parent process: close all pipe file descriptors
    for (int i = 0; i < cmd_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Wait for all child processes to complete
    for (int i = 0; i < cmd_count; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
    
    return 0;
}

// Function to handle pipeline execution in the shell
int handle_pipeline(char *input) {
    Command commands[MAX_PIPE_COMMANDS];
    int cmd_count;
    
    // Parse input into separate commands
    cmd_count = parse_pipeline(input, commands);
    
    if (cmd_count <= 0) {
        fprintf(stderr, "Error: Invalid pipeline command\n");
        return -1;
    }
    
    // Execute pipeline
    int result = execute_pipeline(commands, cmd_count);
    
    // Clean up
    for (int i = 0; i < cmd_count; i++) {
        free_command(&commands[i]);
    }
    
    return result;
}