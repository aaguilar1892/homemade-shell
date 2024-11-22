//redirecting.h
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

void input_redirection(char *cmd, char *inputFile); //Function to implement input redirection
void output_redirection(char *cmd, char *outputFile); //Function to implement output redirection
