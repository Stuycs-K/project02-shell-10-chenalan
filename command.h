#ifndef COMMAND_H
#define COMMAND_H

/*
    Command holds arguments for ONE command (e.g. ls -al)

    char **args: The argument array.
    int arg_count: The current number of arguments. Used for resizing.
    int args_size: The current maximum number of char * in args. Used for resizing.
*/
typedef struct {
    char **args;
    int arg_count;
    int args_size;
} Command;

/*
    CommandChain holds a sequence of commands. The output from each of its commands
    will be piped into the subsequent command.

    The first command's input will be from in_file (default: stdin).
    The last command's output will go to out_file (default: stdout).

    Command **commands: The command array.
    int command_count: The current number of commands. Used for resizing.
    int commands_size: The current maximum number of Command * in commands. Used for resizing.

    char *in_file: stdin will redirect to this. NULL (or stdin) by default.
    char *out_file: stdout will redirect to this. NULL (or stdout) by default.
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

CommandChain *free_command_chain(CommandChain *chain);

#endif
