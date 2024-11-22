/* Name: Corinna Martin
 * Section: 004
 * Date: 11/18/24
 * Description: Supports redirecting stdin and stdout to files
 */
#include "redirecting.h"

//Function implementing input redirection
void input_redirection(char *cmd, char *inputFile){

        int ifp;                //Input file pointer
        pid_t pid = fork();     //Process ID variable

        if(pid > 0){wait(NULL);}
        if(pid == 0){
                //Open input file
                ifp = open(inputFile, O_RDONLY);

                //Duplicate input file to stdin
                dup2(ifp, 0);

                //Close unused file descriptor
                close(ifp);

                //Execute command
                execl("/bin/sh", "/bin/sh", "-c", cmd, (char *)0);
                printf("\n");

                exit(1);
        }
}

//Function implementing output redirection
void output_redirection(char *cmd, char *outputFile){

        int ofp;                //Output file pointer
        pid_t pid = fork();     //Process ID variable

        if(pid > 0){wait(NULL);}
        if(pid == 0){
                //Open output file
                ofp = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

                //Duplicate output file to stdout
                dup2(ofp, 1);

                //Close unused file descriptor
                close(ofp);

                //Execute command
                execl("/bin/sh", "/bin/sh", "-c", cmd, (char *)0);
                printf("\n");

                exit(1);
        }
}
