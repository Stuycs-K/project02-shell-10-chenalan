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
    Reads a line from stdin. The buffer size is dynamic. If fgets encounters
    EOF, the shell terminates.

    PARAMS
        None

    RETURNS
        A heap string containing the read line. Must be freed.
*/
char *read_line() {
    int current_line_size = 2;
    char *line = malloc(sizeof(char) * current_line_size);

    char *fgets_result;
    while (1) {
        fgets_result = fgets(line, current_line_size, stdin);

        if (!fgets_result) {
            if (feof(stdin)) { // Read EOF or Ctrl D, so terminate
                exit(0);
            } else { // Some IO error that isn't my problem

            }
        }

        // If the line read from fgets is too long, we have to resize our buffer
        // and read again
        char *newline_location = strchr(line, '\n');
        if (newline_location) {
            break; // Our buffer has enough room
        }

        current_line_size *= 2;
        line = realloc(line, sizeof(char) * current_line_size);
    }

    printf("%s\n", line);

    strip_newline(line);

    return line;
}
