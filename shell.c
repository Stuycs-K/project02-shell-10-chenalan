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

char *read_line() {
    
}
