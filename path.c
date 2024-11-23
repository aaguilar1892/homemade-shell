#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Global array to store path directories
char *path_list[MAX_PATHS];
int path_count = 0;
char *original_path = NULL;  // Store original PATH for restoration

// Update the system PATH environment variable based on current path_list
void update_system_path() {
    size_t total_len = 0;
    
    // Calculate required length
    for (int i = 0; i < path_count; i++) {
        total_len += strlen(path_list[i]) + 1;  // +1 for : or null terminator
    }

    // Allocate memory for the complete path string
    char *new_path = malloc(total_len);
    if (!new_path) {
        fprintf(stderr, "Error: Memory allocation failed for PATH update\n");
        return;
    }

    // Build the path string
    new_path[0] = '\0';
    for (int i = 0; i < path_count; i++) {
        strcat(new_path, path_list[i]);
        if (i < path_count - 1) {
            strcat(new_path, ":");
        }
    }

    // Update the environment
    if (setenv("PATH", new_path, 1) != 0) {
        fprintf(stderr, "Error updating PATH: %s\n", strerror(errno));
    }

    free(new_path);
}

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
    if (path_count >= MAX_PATHS) {
        fprintf(stderr, "Error: Path limit reached\n");
        return;
    }

    // Check for duplicates
    for (int i = 0; i < path_count; i++) {
        if (strcmp(path_list[i], new_path) == 0) {
            fprintf(stderr, "Warning: Path '%s' already exists\n", new_path);
            return;
        }
    }

    path_list[path_count] = strdup(new_path);
    if (!path_list[path_count]) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    path_count++;
    update_system_path();
}

// Function to remove a path
void remove_path(char *path_to_remove) {
    int found = 0;
    for (int i = 0; i < path_count; i++) {
        if (strcmp(path_list[i], path_to_remove) == 0) {
            free(path_list[i]);
            found = 1;
            
            // Shift remaining paths
            for (int j = i; j < path_count - 1; j++) {
                path_list[j] = path_list[j + 1];
            }
            
            path_count--;
            update_system_path();
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Error: Path '%s' not found\n", path_to_remove);
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
    // Store original PATH for restoration
    original_path = getenv("PATH");
    if (original_path != NULL) {
        original_path = strdup(original_path);
        if (original_path == NULL) {
            fprintf(stderr, "Error: Failed to store original PATH\n");
            return;
        }
    }

    char *env_path = getenv("PATH");
    if (env_path == NULL) {
        // If no PATH is set, set some sensible defaults
        add_path("/usr/local/bin");
        add_path("/usr/bin");
        add_path("/bin");
        update_system_path();
        return;
    }

    char *env_path_copy = strdup(env_path);
    if (!env_path_copy) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    char *token = strtok(env_path_copy, ":");
    while (token != NULL && path_count < MAX_PATHS) {
        add_path(token);
        token = strtok(NULL, ":");
    }

    free(env_path_copy);
    update_system_path();
}

// Restore the original PATH environment variable when exiting
void restore_path() {
    if (original_path) {
        if (setenv("PATH", original_path, 1) != 0) {
            fprintf(stderr, "Error restoring original PATH: %s\n", strerror(errno));
        }
        free(original_path);
    }

    // Free allocated memory
    for (int i = 0; i < path_count; i++) {
        free(path_list[i]);
    }
    path_count = 0;
}