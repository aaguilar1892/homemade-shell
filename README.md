# Major 2 Group Project

## Group Members and Responsibilities

- Corinna Martin: 
    - myhistory command
    - I/O redirection
- Alexis Aguilar: 
    - path command
    - pipelining
- Elisabeth Nguyen: 
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

For the interactive mode, the user is prompted to enter their command into the shell. The input is seperated by the ';' character into individual commands. The command is then passed into the runCommands() function. 

For the batch mode, the filename specified as an argument is opened as an input file stream. The file is read line by line, echoing each line to the terminal. After the line is echoed, the commands is then seperated by ';' (if applicable) and each command is run seperately. The command is then passed to the runCommands() function to execute the given command. The file is read until the end is reached or an exit command is reached.

Both of the modes pass commands to the runCommands() function. In the function, any extra whitespace is removed from the command. If the program encounters an empty command, the shell continues to the next command. Otherwise, the program moves on to check if the command is an alias. If it is, the command is then replaced with the actual command. Then the function checks if the command calls for I/O redirection. If the command string contains either '<' or '>', a function is called to implement either input or output redirection. After this, the program checks if the user's command string matches any one of the built-in commands, and calls the corresponding functions related to them. Finally, if this is not the case, the program forks the parent process and executes the user-entered command inside the child process. In all cases, after the user's command has been executed, it is recorded in the command history.

### Program Description

- preprocessing.c: Removes extra whitespace from user-entered command
- redirecting.c: Implements input & output redirection
- myhistory.c: Manages a list of the executed commands
- alias.c: Manages the alias database with addition, removal, and print functions
- cd.c: Changes the working directory to the user's home or to the specified path
- exit.c: Exits the shell
- path.c: Allows the user to show the current pathname list and append/remove one pathname
- pipelining.c: Implements pipelining
- signal-control.c: Allows the user to stop or pause subprocesses that the shell creates
- main.c: Allows the user to execute commands in the shell entered via the command line or a file

### Complete Specification

**Defensive Programming**
- Incorrect # of command line args when opening shell: The program outputs an error message to stderr and exits.
- Batch input file doesn't exist/fails to open: Program outputs error message to stderr and exits.
- Command not found/couldn't be executed: appropriate error messages are printed to terminal and program continues to next command.
- Input length > 512 characters: Program will output error message and continue to next command by ignoring the rest of the long command.
- Empty command line or ;  ;: command is ignored and continues to next command.
- Extra whitespaces: each command is preprocessed by the preprocessing() function which removes all extra whitespace.
- EOF reached without exit command (Batch Mode): The program ends and no errors are printed.
- Ctrl-D entered as user input in interactive mode: Program checks for fgets() == NULL and exits program if Ctrl-D (EOF) was input.

**myhistory**

If the user enters a command that does not exist, it will still be recorded in the command history. Additionally, when the user executes one of the myhistory commands, only the _myhistory_ command with no flags will be recorded in the command history. Because of this, when the user clears the command history with _myhistory -c_, if they call the _myhistory_ command immediately after this, they will see _myhistory_ as the only command in the history. As for _myhistory -e [command #]_, the command number that it refers to will be recorded in the history in place of the command itself. 

**alias**

If the user tries to define an alias with a name that already exists, an error message is output that the name already exists. To change the command that a name points to, the user would have to remove the existing alias and redefine it.

## Compilation Methods

### Makefile

A Makefile is included in the repository. The purpose of this file is to facilitate the compilation process. To execute the compilation process, use the command `make`.

Executing the command will create an executable file called `newshell`.

To execute the shell in **interactive mode**, type `./newshell` into the terminal.

To execute the shell in **batch mode**, type `./newshell [file]` where `[file]` is the file with the commands to execute.

To clean up any files that were created in the process, use `make clean`.

### Manual Terminal Compilation

To compile the files manually from the terminal, you will have to type out the complete string command. The command is:

`gcc -Wall -Wextra -g main.c preprocessing.c redirecting.c myhistory.c cd.c alias.c path.c pipelining.c exit.c signal-control.c -o newshell`

Executing the command will create an executable file called `newshell`.

To execute the shell in **interactive mode**, type `./newshell` into the terminal.

To execute the shell in **batch mode**, type `./newshell [file]` where `[file]` is the file with the commands to execute.

## Known Issues

When **exit** command is used with other processes it fails to exit the shell correctly.
Additionally, the Signal Handling and Terminal Control was assigned to a specific group member however they failed to implement it correctly.
