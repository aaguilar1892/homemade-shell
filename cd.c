/*Name: Elisabeth Nguyen
 *Command: cd (Change Directory)
 *Description:  cd() takes string input, changes directory to provided path or to the user's home if path not provided, and calls printCwd()
 *              printCwd() prints the current working directory
*/
#include "cd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void cd(char* newd) {
     if (newd == NULL) {
        //if path not provided, change directory to home
        char* user_home = getenv("HOME");
        chdir(user_home);
    }
    else {
        //if path provided, change directory
        chdir(newd);
    }
    //print current working directory
    printCwd();
}
void printCwd() {
    char cwd[300];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
    } 
    
    else {
        perror("getcwd() error");
    }
}
