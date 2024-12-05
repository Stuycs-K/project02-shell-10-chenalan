#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"

Command *new_command() {
    Command *command = malloc(sizeof(Command));

    command->arg_count = 0;
    command->args_size = 64;
    command->args = malloc(sizeof(char *) * command->args_size);

    return command;
}

void insert_arg(Command *command, char *arg) {
    if (command->arg_count >= command->args_size) {
        command->args_size *= 2;
        command->args = realloc(command->args, sizeof(char *) * command->args_size);
    }

    command->args[command->arg_count++] = arg;
}

Command *free_command(Command *command) {
    free(command->args);

    free(command);

    return NULL;
}

CommandChain *new_command_chain() {
    CommandChain *command_chain = malloc(sizeof(CommandChain));

    command_chain->command_count = 0;
    command_chain->commands_size = 64;
    command_chain->commands = malloc(sizeof(Command *) * command_chain->commands_size);

    return command_chain;
}

void insert_command(CommandChain *chain, Command *command) {
    if (chain->command_count >= chain->commands_size) {
        chain->commands_size *= 2;
        chain->commands = realloc(chain->commands, sizeof(Command *) * chain->commands_size);
    }

    chain->commands[chain->command_count++] = command;
}

Command *last_command(CommandChain *chain) {
    if (chain->command_count == 0) {
        return NULL;
    }

    return chain->commands[chain->command_count - 1];
}

void set_in_file(CommandChain *chain, char *file_name) {
    chain->in_file = malloc(sizeof(char) * strlen(file_name));
    strcpy(chain->in_file, file_name);
}

void set_out_file(CommandChain *chain, char *file_name) {
    chain->out_file = malloc(sizeof(char) * strlen(file_name));
    strcpy(chain->out_file, file_name);
}

CommandChain *free_command_chain(CommandChain *chain) {
    for (int i = 0; i < chain->command_count; ++i) {
        chain->commands[i] = free_command(chain->commands[i]);
    }

    free(chain->in_file);
    free(chain->out_file);
    free(chain);

    return NULL;
}