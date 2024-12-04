#include <stddef.h>
#include <string.h>

#include "parse.h"

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

/*
    Parses a string possibly containing multiple commands separated by semicolons
    into a null-terminated command array.

    Each command string can then be parsed into the command and its arguments
    with parse_command_args

    PARAMS
        char *line: The string to be parsed.

        char **arg_array: Contains the semicolon-separated tokens (i.e. commands)
            in line. It will be null-terminated.

    RETURNS
        void
*/
void parse_commands(char *line, char **command_array) {
    char *current = line;
    char *token;

    int index = 0;

    while (token = strsep(&current, ";")) {
        command_array[index++] = token;
    }

    command_array[index] = NULL;
}

char *parse_stdin_redirect(char **arg_array) {
    char **current = arg_array;

    while (*current) {
        if (!strcmp(*current, "<")) {
            return *(current + 1); // The next string in the argument array will be the file name
        }
        current++;
    }

    return NULL;
}

char *parse_stdout_redirect(char **arg_array) {
    char **current = arg_array;

    while (*current) {
        if (!strcmp(*current, ">")) {
            return *(current + 1); // The next string in the argument array will be the file name
        }
        current++;
    }

    return NULL;
}