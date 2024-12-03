#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "cd.h"
#include "exec.h"
#include "input.h"

static void ignore_shell_signal(int signo) {
    printf("\n");
}

int main(int argc, char *argv[]) {
    signal(SIGINT, ignore_shell_signal);
    signal(SIGQUIT, ignore_shell_signal);

    while (1) {
        char user_input[512];

        char *wd = get_wd_absolute();
        shorten_homedir_in_path(wd);

        // If stdin's been redirected, don't output the prompt!
        if (isatty(0)) {
            printf("%s$ ", wd);
            fflush(stdout);
        }

        char *fgets_result = fgets(user_input, sizeof(user_input), stdin);
        if (!fgets_result) {
            if (feof(stdin)) {
                exit(0);
            }
        }

        strip_newline(user_input);

        char *commands[128];
        parse_commands(user_input, commands);

        char **current_command = commands;
        while (*current_command) {
            char *arg_array[128];
            parse_command_args(*current_command, arg_array);
            exec(arg_array);

            current_command++;
        }

        free(wd);
    }

    return 0;
}
