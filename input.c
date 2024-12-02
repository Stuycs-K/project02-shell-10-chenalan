#include <string.h>
#include <stddef.h>

#include "input.h"

void parse_command_args(char *line, char **arg_array) {
    char *current = line;
    char *token;

    int arg_id = 0;

    while (token = strsep(&current, " ")) {
        arg_array[arg_id++] = token;
    }

    arg_array[arg_id] = NULL;
}
