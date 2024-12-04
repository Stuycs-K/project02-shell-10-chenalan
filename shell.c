#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "cd.h"
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

    strip_newline(line);

    return line;
}

/*
    Executes each command.

    PARAMS
        char **command_array: An array of commands, not yet separated into arguments.

    RETURNS
        None.
*/
void run_commands(char **command_array) {
    char **current_command = command_array;
    while (*current_command) {
        char *arg_array[128];
        parse_command_args(*current_command, arg_array);

        int exit_status = exec(arg_array);

        // TODO: Handle errors

        current_command++;
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

    output_prompt();

    line = read_line();

    char *commands[256];
    parse_commands(line, commands);

    shell_status = SHELL_STATUS_EXEC;
    run_commands(commands);
    shell_status = SHELL_STATUS_IDLE;

    free(line);
}
