#ifndef PATH_H
#define PATH_H

#include <stdio.h>

// Maximum number of paths to store
#define MAX_PATHS 100

// Declare an array to store the paths
extern char *path_list[MAX_PATHS];
extern int path_count;

// Function declarations for path built-in functionality
void show_paths();
void add_path(char *new_path);
void remove_path(char *path_to_remove);
void path_builtin(char **args);
void initialize_path();
void restore_path();

#endif /* PATH_H */