#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "redirection.h"

/*
    Redirects stdin to the given file.

    PARAMS
        char *file_name: The file that will replace stdin.

    RETURNS
        None.
*/
void redirect_stdin(char *file_name) {
    int handle = open(file_name, O_RDONLY);
    if (handle < 0) {
        perror("Could not open input file");
        exit(errno);
    }

    dup(STDIN_FILENO);

    dup2(handle, STDIN_FILENO);

    close(handle);
}

/*
    Redirects stdout to the given file.

    PARAMS
        char *file_name: The file that will replace stdout.
            Will be created if it does not exist.

    RETURNS
        None.
*/
void redirect_stdout(char *file_name) {
    int handle = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (handle < 0) {
        perror("Could not create output file");
        exit(errno);
    }

    dup(STDOUT_FILENO);

    dup2(handle, STDOUT_FILENO);

    close(handle);
}