//Elisabeth Nguyen
#include "cd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void cd(char* newd) {
     if (newd == NULL) {
        char* user_home = getenv("HOME");
        chdir(user_home);
         printf("changed to home\n");
    }
    else {
        chdir(newd);
        printf("changed to %s\n", newd);
        
        char cwd[300];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
    }
}
