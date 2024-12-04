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
        The file descriptor for the copied stdin.
*/
int redirect_stdin(char *file_name) {
    int handle = open(file_name, O_RDONLY);
    if (handle < 0) {
        perror("Could not open input file");
        exit(errno);
    }

    int backup_stdin = dup(STDIN_FILENO);

    dup2(handle, STDIN_FILENO);

    close(handle);

    return backup_stdin;
}

/*
    Redirects stdout to the given file.

    PARAMS
        char *file_name: The file that will replace stdout.
            Will be created if it does not exist.

    RETURNS
        The file descriptor for the copied stdout.
*/
int redirect_stdout(char *file_name) {
    int handle = open(file_name, O_CREAT | O_WRONLY, 0644);
    if (handle < 0) {
        perror("Could not create output file");
        exit(errno);
    }

    int backup_stdout = dup(STDOUT_FILENO);

    dup2(handle, STDOUT_FILENO);

    close(handle);

    return backup_stdout;
}