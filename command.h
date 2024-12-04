#ifndef COMMAND_H
#define COMMAND_H

/*
    Command holds arguments for ONE command (e.g. ls -al) and redirect information
    So ls | wc will be split into two Commands
*/
typedef struct {
    char **args;
    int arg_count;
    int args_size;

    char *in_file;
    char *out_file;

    int in_file_fd;
    int out_file_fd;
} Command;

/*
    CommandChain is just a list of commands
*/
typedef struct {
    Command **commands;
    int command_count;
    int commands_size;
} CommandChain;

Command *new_command();

void insert_arg(Command *command, char *arg);

void set_in_file(Command *command, char *file_name);

void set_out_file(Command *command, char *file_name);

Command *free_command(Command *command);

CommandChain *new_command_chain();

void insert_command(CommandChain *chain, Command *command);

Command *last_command(CommandChain *chain);

CommandChain *free_command_chain(CommandChain *chain);

#endif
