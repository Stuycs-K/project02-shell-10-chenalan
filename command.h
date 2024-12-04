#ifndef COMMAND_H
#define COMMAND_H

/*
    Command holds information for redirects.
*/
typedef struct {
    char **args;

    char *in_file;
    char *out_file;

    int in_file_fd;
    int out_file_fd;
} Command;

#endif
