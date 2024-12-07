#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"

/*
    Constructs a new command. Initializes its argument array.

    PARAMS
        None.

    RETURNS
        The created command.
*/
Command *new_command() {
    Command *command = malloc(sizeof(Command));

    command->arg_count = 0;
    command->args_size = 64;
    command->args = malloc(sizeof(char *) * command->args_size);

    return command;
}

/*
    Inserts an argument into the command's argument array.
    Resizes if necessary.

    PARAMS
        Command *command: The command.
        char *arg: The argument to insert.

    RETURNS
        None
*/
void insert_arg(Command *command, char *arg) {
    if (command->arg_count >= command->args_size) {
        command->args_size *= 2;
        command->args = realloc(command->args, sizeof(char *) * command->args_size);
    }

    command->args[command->arg_count++] = arg;
    command->args[command->arg_count] = NULL; // MUST KEEP ARGS NULL-TERMINATED!
}

/*
    Cleans up the command.

    PARAMS
        Command *command: The command.

    RETURNS
        NULL.
*/
Command *free_command(Command *command) {
    for (int i = 0; i < command->arg_count; ++i) {
        free(command->args[i]);
    }
    free(command->args);

    free(command);

    return NULL;
}

/*
    Constructs a new command chain. Initializes its commands array.

    PARAMS
        None.

    RETURNS
        The new command chain.
*/
CommandChain *new_command_chain() {
    CommandChain *command_chain = malloc(sizeof(CommandChain));

    command_chain->command_count = 0;
    command_chain->commands_size = 64;
    command_chain->commands = malloc(sizeof(Command *) * command_chain->commands_size);

    command_chain->in_file = NULL;
    command_chain->out_file = NULL;

    return command_chain;
}

/*
    Inserts a command into the chain's command array. Resizes if necessary.

    PARAMS
        CommandChain *chain: The command chain.
        Command *command: The command to insert.

    RETURNS
        None.
*/
void insert_command(CommandChain *chain, Command *command) {
    if (chain->command_count >= chain->commands_size) {
        chain->commands_size *= 2;
        chain->commands = realloc(chain->commands, sizeof(Command *) * chain->commands_size);
    }

    chain->commands[chain->command_count++] = command;
}

/*
    Sets the command chain's redirect file for stdin. If a redirect file is
    already set, it is overridden.

    PARAMS
        CommandChain *chain: The command chain.
        char *file_name: A string containing the redirect file. Will be copied.

    RETURNS
        None.
*/
void set_in_file(CommandChain *chain, char *file_name) {
    if (chain->in_file) {
        free(chain->in_file);
    }
    chain->in_file = malloc(sizeof(char) * strlen(file_name));

    strcpy(chain->in_file, file_name);
}

/*
    Sets the command chain's redirect file for stdout. If a redirect file is
    already set, it is overridden.

    PARAMS
        CommandChain *chain: The command chain.
        char *file_name: A string containing the redirect file. Will be copied.

    RETURNS
        None.
*/
void set_out_file(CommandChain *chain, char *file_name) {
    if (chain->out_file) {
        free(chain->out_file);
    }
    chain->out_file = malloc(sizeof(char) * strlen(file_name));

    strcpy(chain->out_file, file_name);
}

/*
    Cleans up the command chain and calls free_command on all its commands.

    PARAMS
        CommandChain *chain: The command chain.

    RETURNS
        NULL.
*/
CommandChain *free_command_chain(CommandChain *chain) {
    for (int i = 0; i < chain->command_count; ++i) {
        chain->commands[i] = free_command(chain->commands[i]);
    }

    free(chain->in_file);
    free(chain->out_file);
    free(chain);

    return NULL;
}
