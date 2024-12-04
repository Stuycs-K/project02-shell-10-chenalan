#ifndef COMMAND_H
#define COMMAND_H

/*
    Command holds arguments for ONE command (e.g. ls -al) and redirect information
    So ls | wc will be split into two Commands
*/
typedef struct {
    char **args;

    char *in_file;
    char *out_file;

    int in_file_fd;
    int out_file_fd;
} Command;

Command *new_command();

void insert_arg(Command *command, char *arg);

#endif
