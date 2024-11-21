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
