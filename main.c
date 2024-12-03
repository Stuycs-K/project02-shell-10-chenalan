#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "cd.h"
#include "exec.h"
#include "input.h"
#include "shell.h"

static void ignore_shell_signal(int signo) {
    printf("\n");
}

int main(int argc, char *argv[]) {
    signal(SIGINT, ignore_shell_signal);
    signal(SIGQUIT, ignore_shell_signal);

    char *line;

    while (1) {
        output_prompt();

        line = read_line();

        char *commands[128];
        parse_commands(line, commands);

        run_commands(commands);

        free(line);
    }

    return 0;
}
