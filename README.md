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

TODO: A few paragraphs describing the overall structure of your code and any important structures...

### Program Description

- filename: description...

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
