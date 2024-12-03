#include <errno.h>
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
    Runs the cd command. Changes the working directory to the one requested,
    returning errno if something goes wrong.

    PARAMS
        char **argv: The argument array normally passed into execvp. The first
            argument should be "cd," and the next could either contain NULL or
            a path

    RETURNS
        None, but exits with either errno or 0
*/
void cd(char **argv) {
    char *path = argv[1];
    // TODO: If we just call "cd", we go back to the home directory
    if (!path) {
        path = "~";
    }

    int result = chdir(path);

    if (result == -1) {
        exit(errno);
    }

    exit(0);
}