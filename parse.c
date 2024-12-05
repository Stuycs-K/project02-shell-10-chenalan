#include <stddef.h>
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

    char *expanded_line = malloc(sizeof(char) * line_size * 3);
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
    expanded_line[index++] = ' ';
    expanded_line[index++] = 0;

    free(line);

    return expanded_line;
}

/*
    Inserts whitespace-separated tokens into an array.

    PARAMS
        char *line: The input line.

        char **tokens: The array into which each token will be inserted.

    RETURNS
        None
*/
void separate_tokens(char *line, char **tokens) {
    char *current = line;

    char *token = NULL;
    int token_length = 0;

    int index = 0;

    while (*current) {
        // Whitespace -> insert current token
        if (*current == ' ') {
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

            Command *command = last_command(current_chain);
            char *file_name = *(++token_pointer); // Consume next token as file input
            set_in_file(command, file_name);

            build_new_command_flag = 1;

        } else if (!strcmp(current_token, ">")) { // Redirect stdout

            Command *command = last_command(current_chain);
            char *file_name = *(++token_pointer); // Consume next token as file input
            set_out_file(command, file_name);

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
                command = last_command(current_chain);
            }

            // Add to args list
            insert_arg(command, current_token);
        }

        token_pointer++;
    }

    return command_chains;
}
