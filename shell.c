#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "shell.h"
#include "cd.h"
#include "exec.h"
#include "input.h"

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
    if (!isatty(0)) {
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

char **get_command_array(char *line) {
    char *commands[128];
    parse_commands(line, commands);
}

void run_commands(char **command_array) {
    char **current_command = command_array;
    while (*current_command) {
        char *arg_array[128];
        parse_command_args(*current_command, arg_array);
        exec(arg_array);

        current_command++;
    }
}
