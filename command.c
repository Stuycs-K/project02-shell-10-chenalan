#include <stdlib.h>
#include <string.h>

#include "command.h"

Command *new_command() {
    Command *command = malloc(sizeof(Command));

    command->in_file = NULL;
    command->out_file = NULL;

    return command;
}

void set_in_file(Command *command, char *file_name) {
    command->in_file = malloc(sizeof(char) * strlen(file_name));
    strcpy(command->in_file, file_name);
}

void set_out_file(Command *command, char *file_name) {
    command->out_file = malloc(sizeof(char) * strlen(file_name));
    strcpy(command->out_file, file_name);
}

void free_command(Command *command) {
    free(command->in_file);
    free(command->out_file);

    free(command);
}
