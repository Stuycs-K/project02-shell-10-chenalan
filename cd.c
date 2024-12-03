#include <errno.h>
#include <pwd.h>
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
        None
*/
void replace_homedir_in_path(char *wd) {
    char *homedir = get_homedir();
    char *homedir_location = strstr(wd, homedir);

    // Either there's no home directory, or it's not at the beginning of the path
    if (!homedir_location || homedir_location != wd) {
        free(homedir);
        return;
    }

    char *remaining_path = wd + strlen(homedir);

    wd[0] = '~';
    wd[1] = 0;
    strcat(wd, remaining_path);

    free(homedir);
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
        Either errno or 0
*/
int cd(char **argv) {
    char *path = argv[1];
    char *homedir = NULL;

    int result = 0;

    // "cd" or "cd ~"
    if (!path || !strcmp(path, "~")) {
        result = chdir(get_homedir());
    } else {
        result = chdir(path);
    }

    if (homedir) {
        free(homedir);
    }

    if (result == -1) {
        return errno;
    }

    return 0;
}