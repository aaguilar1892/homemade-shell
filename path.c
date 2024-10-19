#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global array to store path directories
char *path_list[MAX_PATHS];
int path_count = 0;

// Function to display the current path list
void show_paths() {
    for (int i = 0; i < path_count; i++) {
        printf("%s", path_list[i]);
        if (i < path_count - 1) {
            printf(":");  // Separate paths with colon
        }
    }
    printf("\n");
}

// Function to add a new path
void add_path(char *new_path) {
    if (path_count < MAX_PATHS) {
        path_list[path_count] = strdup(new_path); // Duplicate the string to store it
        path_count++;
    } else {
        fprintf(stderr, "Error: Path limit reached, cannot add more paths.\n");
    }
}

// Function to remove a path
void remove_path(char *path_to_remove) {
    int found = 0;
    for (int i = 0; i < path_count; i++) {
        if (strcmp(path_list[i], path_to_remove) == 0) {
            free(path_list[i]);  // Free the memory for the removed path
            found = 1;
            // Shift remaining paths down by one
            for (int j = i; j < path_count - 1; j++) {
                path_list[j] = path_list[j + 1];
            }
            path_list[path_count - 1] = NULL;
            path_count--;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Error: Path '%s' not found.\n", path_to_remove);
    }
}

// Function to handle the path built-in command
void path_builtin(char **args) {
    if (args[1] == NULL) {
        // No arguments: show current paths
        show_paths();
    } else if (strcmp(args[1], "+") == 0 && args[2] != NULL) {
        // Add a new path
        add_path(args[2]);
    } else if (strcmp(args[1], "-") == 0 && args[2] != NULL) {
        // Remove a path
        remove_path(args[2]);
    } else {
        fprintf(stderr, "Error: Invalid usage of path command.\n");
        fprintf(stderr, "Usage: path            # show paths\n");
        fprintf(stderr, "       path + <dir>    # add directory to paths\n");
        fprintf(stderr, "       path - <dir>    # remove directory from paths\n");
    }
}

// Initialize the path list with current environment's PATH variable
void initialize_path() {
    char *env_path = getenv("PATH");
    if (env_path != NULL) {
        char *token = strtok(env_path, ":");
        while (token != NULL) {
            add_path(token);
            token = strtok(NULL, ":");
        }
    }
}

// Restore the original PATH environment variable when exiting
void restore_path() {
    // Build a string from the path_list array
    char restored_path[1024] = "";
    for (int i = 0; i < path_count; i++) {
        strcat(restored_path, path_list[i]);
        if (i < path_count - 1) {
            strcat(restored_path, ":");
        }
    }
    setenv("PATH", restored_path, 1);  // Restore the PATH environment variable
}