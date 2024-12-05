#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "cd.h"
#include "command.h"
#include "exec.h"
#include "parse.h"
#include "shell.h"

/*
    Flag for whether the shell is currently running a command
    IDLE: 0
    EXEC: 1
*/
int shell_status = SHELL_STATUS_IDLE;

/*
    Prints the shell prompt. If stdin has been redirected, this prints nothing.
    The home directory, if present, is replaced with "~".

    PARAMS
        None

    RETURNS
        None
*/
void output_prompt() {
    // If stdin's been redirected, don't output the prompt!
    if (!isatty(STDIN_FILENO)) {
        return;
    }

    char *wd = get_wd_absolute();
    shorten_homedir_in_path(wd);

    printf("%s$ ", wd);
    fflush(stdout);

    free(wd);
}

/*
    Reads a line from stdin. If fgets encounters EOF, the shell terminates.

    PARAMS
        None

    RETURNS
        A heap string containing the read line. Must be freed.
*/
char *read_line() {
    char *line = malloc(sizeof(char) * 512);

    char *fgets_result = fgets(line, 512, stdin);
    if (!fgets_result) {
        if (feof(stdin)) {
            exit(0);
        }
    }

    int location = strlen(line) - 1;
    char final_char = line[location];

    if (final_char == '\n') {
        line[location] = 0;
    }

    return line;
}

/*
    Executes each command.

    PARAMS
        char **command_array: An array of commands, not yet separated into arguments.

    RETURNS
        None.
*/
void run_commands(CommandChain **command_chains) {
    CommandChain **current_chain = command_chains;
    while (*current_chain) {
        exec_chain(*current_chain);

        current_chain++;
    }
}

/*
    Runs the main loop of the shell. Outputs the prompt, reads user input, parses,
    and runs commands.

    shell_status is set to EXEC before run_commands and back to IDLE after it resolves.

    PARAMS
        None.

    RETURNS
        None.
*/
void shell_loop() {
    char *line;
    char *tokens[2048];

    CommandChain **command_chains;

    output_prompt();

    line = read_line();
    line = format_line(line);
    separate_tokens(line, tokens);

    command_chains = build_command_chains(tokens);

    shell_status = SHELL_STATUS_EXEC;
    run_commands(command_chains);
    shell_status = SHELL_STATUS_IDLE;

    free(line);

    // Cleanup tokens
    char **token_pointer = tokens;
    while (*token_pointer) {
        free(*token_pointer);
        token_pointer++;
    }

    // Cleanup command chains
    CommandChain **chain = command_chains;
    while (*chain) {
        free_command_chain(*chain);
        chain++;
    }
}
