#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "parse.h"

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

    char *expanded_line = malloc(sizeof(char) * (line_size + 2) * 2);
    int index = 0;

    int is_building_string_literal = 0; // For " "

    for (int i = 0; i < line_size; ++i) {
        char current = line[i];

        // Encountering " or ' -> don't treat anything after as a special token
        if (current == '\'' || current == '\"') { 
            is_building_string_literal = is_building_string_literal ? 0 : 1;
        }

        // Insert spaces between special tokens for easy splitting
        if (!is_building_string_literal && (current == '|' || current == '<' || current == '>' || current == ';')) {
            expanded_line[index++] = ' ';
            expanded_line[index++] = current;
            expanded_line[index++] = ' ';
        } else {
            expanded_line[index++] = current;
        }
    }
    expanded_line[index++] = ' ';
    expanded_line[index++] = 0;

    free(line);

    return expanded_line;
}

/*
    Inserts whitespace-separated tokens from an input line into an array.
    The tokens can be separated with any number of spaces.

    PARAMS
        char *line: The input line.

        char **tokens: The array into which each token will be inserted.

    RETURNS
        None.
*/
void separate_tokens(char *line, char **tokens) {
    char *current = line;

    char *token = NULL;
    int token_length = 0;

    int index = 0;

    int is_building_string_literal = 0;

    while (*current) {
        // Toggle string literal flag
        if (*current == '\'' || *current == '\"') {
            is_building_string_literal = is_building_string_literal ? 0 : 1;

            // Skip the quotes themselves
            current++;
            continue;
        }

        // Whitespace -> insert current token
        if (!is_building_string_literal && *current == ' ') {
            if (token_length > 0) {
                // +1 for null
                char *string = malloc(sizeof(char) * (token_length + 1));

                // Copy the token and null out the final byte
                strncpy(string, token, token_length);
                string[token_length] = 0;

                tokens[index++] = string;

                token_length = 0;
            }

            // Reset token pointer
            token = NULL;

            current++;
            continue;
        }

        // Set pointer to start of first string
        if (token == NULL) {
            token = current;
        }

        // Increment token length
        token_length++;

        current++;
    }

    tokens[index] = NULL;
}

/*
    The main parse function. Builds a list of command chains from a token list.
    If we encounter ";", begin building a new command chain.
    If we encounter "|", set a flag that the next word will go towards a new command.
    If we encounter "<" or ">", set the input and output files of the current command chain.

    Any other word is inserted to the current command's argument list.

    PARAMS
        char **tokens: The tokens.

    RETURNS
        An null-terminated array of command chains, each of which can be executed.
        Must be freed.
*/
CommandChain **build_command_chains(char **tokens) {
    CommandChain **command_chains = calloc(sizeof(CommandChain *), 512);
    command_chains[0] = new_command_chain(); // Initialize 1 chain

    int current_command_chain = 0; // Which command chain

    int build_new_command_flag = 1; // Should we build a new command? Set to 1 after reading a special character

    char **token_pointer = tokens;
    char *current_token;

    while (current_token = *token_pointer) {
        CommandChain *current_chain = command_chains[current_command_chain];

        if (!strcmp(current_token, "|")) { // Pipe

            build_new_command_flag = 1;

        } else if (!strcmp(current_token, "<")) { // Redirect stdin

            char *next_token = *(++token_pointer);

            if (!next_token) {
                perror("shell: syntax error");
                return command_chains;
            }

            set_in_file(current_chain, next_token);

            build_new_command_flag = 1;

        } else if (!strcmp(current_token, ">")) { // Redirect stdout

            char *next_token = *(++token_pointer);

            if (!next_token) {
                perror("shell: syntax error");
                return command_chains;
            }

            set_out_file(current_chain, next_token);

            build_new_command_flag = 1;

        } else if (!strcmp(current_token, ";")) { // Build a new command chain

            command_chains[++current_command_chain] = new_command_chain();

            build_new_command_flag = 1; // Next token (if any) we read should go to a new command

        } else { // This is a command argument

            Command *command;

            if (build_new_command_flag) {
                command = new_command();

                insert_command(current_chain, command);

                build_new_command_flag = 0;
            } else { // If we're not building a new command, keep adding onto the last command's arg list
                command = current_chain->commands[current_chain->command_count - 1];
            }

            // Add to args list
            insert_arg(command, current_token);
        }

        token_pointer++;
    }

    command_chains[++current_command_chain] = NULL;
    return command_chains;
}
