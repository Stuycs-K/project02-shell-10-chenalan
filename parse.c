#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "command.h"

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

/*
    Adds spaces between special characters in the command line for easy separation.
    The special characters are |, <, >, and ;

    PARAMS
        char *line: The input line.

    RETURNS
        A modified heap string, with whitespace between each found special character.
        Must be freed.
*/
char *format_line(char *line) {
    int line_size = strlen(line);

    char *expanded_line = malloc(sizeof(char) * line_size * 2);
    int index = 0;

    for (int i = 0; i < line_size; ++i) {
        char current = line[i];

        // Insert spaces between special tokens for easy splitting
        if (current == '|' || current == '<' || current == '>' || current == ';') {
            expanded_line[index++] = ' ';
            expanded_line[index++] = current;
            expanded_line[index++] = ' ';
        } else {
            expanded_line[index++] = current;
        }
    }

    free(line);

    return expanded_line;
}

char *separate_tokens(char *line, char **tokens) {
    char *current = line;
    char *token;

    int index = 0;

    while (token = strsep(&current, " ")) {
        tokens[index++] = token;
    }

    tokens[index] = NULL;
}

Command **build_command_arrays(char **tokens) {
    Command ***command_structure = malloc(sizeof(Command **) * 512);

    int current_command_chain = 0; // Which Command list (Command **)
    int current_command = 0; // Which Command in the list (Command *)

    int build_new_command_flag = 1; // Should we build a new command? Set to 1 after reading a special character

    char **token_pointer = tokens;
    char *current_token;

    while (current_token = *token_pointer) {
        if (!strcmp(current_token, "|")) { // PIPE

        } else if (!strcmp(current_token, "<")) {

        } else if (!strcmp(current_token, ">")) {

        } else if (!strcmp(current_token, ";")) { // Build a new command array
            command_structure[current_command_chain++] = malloc(sizeof(Command *) * 512);
            build_new_command_flag = 1; // Next token we read should go to a new command
        } else {
            Command *command;

            if (build_new_command_flag) {
                command = new_command();

                // Advance command count for the current chain
                command_structure[current_command_chain][++current_command] = command;

                build_new_command_flag = 0;
            } else {
                command = command_structure[current_command_chain][current_command];
            }

            // Add to args list
            insert_arg(command, current_token);
        }

        token_pointer++;
    }
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
