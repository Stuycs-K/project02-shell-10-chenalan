#include <stdlib.h>
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

void cd(char *path) {
    return;
}