# Major 2 Group Project

## Group Members and Responsibilities

- Corinna Martin: 
    - myhistory command
    - I/O redirection
- Alexis Aguilar: 
    - path command
    - pipelining
- Elizabeth Nguyen: 
    - cd command
    - alias command
- Nelli Salinas: 
    - exit command
    - signal handling & terminal control
- All: 
    - Interactive mode
    - Batch mode
    - Defensive programming
    - Makefile
    - README file

## Design Overview

The program takes two parameters, int argc, and char* argv[], representing the argument count and the command line arguments themselves. 

If the argument count is equal to 1, the program enters an interactive mode where the user can type their commands in the command line until they choose to exit the program. If the argument count is equal to 2, the program enters a batch mode where commands are executed from the file specified by the user when the program was called. If the argument count is not equal to either of these two values, the program prints a usage statement informing the user of the correct way to call it before exiting.

For the interactive mode, the user is prompted to enter their command into the shell. Any extra whitespace is removed from the user's command. If the user entered an empty command line, the shell immediately prompts for another command. Otherwise, the program moves on to check if the command calls for I/O redirection. If the command string contains either '<' or '>', a function is called to implement either input or output redirection. After this, the program checks if the user's command string matches any one of the built-in commands, and calls the corresponding functions related to them. Finally, if this is not the case, the program forks the parent process and executes the user-entered command inside the child process. In all cases, after the user's command has been executed, it is recorded in the command history.

TODO... (Batch Mode)

### Program Description

- filename: description...
- preprocessing.c: Removes extra whitespace from user-entered command
- redirecting.c: Implements input & output redirection
- myhistory.c: Manages a list of the executed commands
- ...

### Complete Specification

TODO: Describe how you handled any ambiguities in the specification. For example, for this project, explain how your shell will handle lines that have no commands between semi-colons...

**main**

TODO...

**myhistory**

If the user enters a command that does not exist, it will still be recorded in the command history. Additionally, when the user executes one of the myhistory commands, only the _myhistory_ command with no flags will be recorded in the command history. Because of this, when the user clears the command history with _myhistory -c_, if they call the _myhistory_ command immediately after this, they will see _myhistory_ as the only command in the history. As for _myhistory -e [command #]_, the command number that it refers to will be recorded in the history in place of the command itself. 

## Compilation Methods

### Makefile

A Makefile is included in the repository. The purpose of this file is to facilitate the compilation process. To execute the compilation process, use the command `make`.

To clean up any files that were created in the process, use `make clean`.

### Manual Terminal Compilation

To compile the files manually from the terminal, you will have to type out the complete string command. The command is:

`...`

Executing the command will create an executable file called `...`.

To execute it, type `...` into the terminal.

## Known Issues

Currently no issues are known. Issues may arise as we further develop this project.
