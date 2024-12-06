#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"

/*
    Returns a string containing the absolute path to the working directory

    PARAMS
        None

    RETURNS
        A heap string with the absolute path. Must be freed.
*/
char *get_wd_absolute(void) {
    int path_size = 64;

    char *path_buffer = malloc(sizeof(char) * path_size);

    // Keep resizing the buffer until the path fits
    while (!getcwd(path_buffer, path_size)) {
        path_size *= 2;
        path_buffer = realloc(path_buffer, path_size);
    }

    return path_buffer;
}

/*
    Returns a string containing the home directory.
    Source: https://stackoverflow.com/questions/2910377/get-home-directory-in-linux

    PARAMS
        None

    RETURNS
        A heap string with the home directory. Must be freed.
*/
char *get_homedir(void) {
    char *homedir = getenv("HOME");

    if (!homedir) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    char *path_buffer = malloc(sizeof(char) * strlen(homedir));
    strcpy(path_buffer, homedir);

    return path_buffer;
}

/*
    Shortens a path by replacing the home directory (if found) with "~".

    PARAMS
        char *wd: The buffer containing the absolute path. Will be modified.

    RETURNS
        None. There's no need to create a new buffer, as path will
        have enough space for the shortened path.
*/
void shorten_homedir_in_path(char *path) {
    char *homedir = get_homedir();
    char *homedir_location = strstr(path, homedir);

    // Either there's no home directory, or it's not at the beginning of the path
    if (!homedir_location || homedir_location != path) {
        free(homedir);
        return;
    }

    char *remaining_path = path + strlen(homedir);

    path[0] = '~';
    path[1] = 0;
    strcat(path, remaining_path);

    free(homedir);
}

/*
    Used internally. Expands "~" in a path by replacing it with the home directory.

    PARAMS
        char *path: The buffer containing the path relative to the home directory.

    RETURNS
        A new heap buffer containing the expanded path. Must be freed.
        If the path does not contain "~", returns a copy of the path passed in.
*/
char *expand_homedir_in_path(char *path) {
    if (path[0] != '~') {
        char *copy = malloc(sizeof(char) * strlen(path));
        strcpy(copy, path);

        return copy;
    }

    char *homedir = get_homedir();

    char *remaining_path = path + 1;

    char *expanded_path = malloc(sizeof(char) * (strlen(homedir) + strlen(remaining_path)));
    strcpy(expanded_path, homedir);
    strcat(expanded_path, remaining_path);

    free(homedir);

    return expanded_path;
}

/*
    Runs the cd command. Changes the working directory to the one requested,
    returning errno if something goes wrong. If no path is specified,
    the path will be the home directory. "~" at the beginning of a path will be expanded
    into the home directory.

    PARAMS
        char **argv: The argument array normally passed into execvp. The first
            argument should be "cd," and the next could either contain NULL or
            a path.

    RETURNS
        None.
*/
void cd(char **argv) {
    char *path = argv[1];

    char *expanded_path = NULL;

    // "cd" becomes "cd ~"
    if (!path) {
        expanded_path = expand_homedir_in_path("~");
    } else {
        expanded_path = expand_homedir_in_path(path);
    }

    int result = chdir(expanded_path);

    free(expanded_path);

    if (result == -1) {
        perror("[cd]");
    }
}
