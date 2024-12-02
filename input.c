#include <string.h>
#include <stddef.h>

#include "input.h"

/*
    Parses the arguments for a single command and its arguments into a
    null-terminated argument array, which can then be passed into execvp.

    The command name is always index 0.

    PARAMS
        char *line: The string to be parsed.

        char **arg_array: Contains the whitespace-separated tokens in line. 
            It will be null-terminated.

    RETURNS
        void
*/
void parse_command_args(char *line, char **arg_array) {
    char *current = line;
    char *token;

    int arg_id = 0;

    while (token = strsep(&current, " ")) {
        arg_array[arg_id++] = token;
    }

    arg_array[arg_id] = NULL;
}
