#ifndef COMMAND_H
#define COMMAND_H

/*
    Command holds arguments for ONE command (e.g. ls -al)
*/
typedef struct {
    char **args;
    int arg_count;
    int args_size;
} Command;

/*
    CommandChain holds a list of Commands. The output from each Command
    will be piped into the subsequent Command.

    It also stores redirect information: the initial stdin and final stdout file paths.
*/
typedef struct {
    Command **commands;
    int command_count;
    int commands_size;

    char *in_file;
    char *out_file;
} CommandChain;

Command *new_command();

void insert_arg(Command *command, char *arg);

void set_in_file(CommandChain *command, char *file_name);

void set_out_file(CommandChain *command, char *file_name);

Command *free_command(Command *command);

CommandChain *new_command_chain();

void insert_command(CommandChain *chain, Command *command);

Command *last_command(CommandChain *chain);

CommandChain *free_command_chain(CommandChain *chain);

#endif
