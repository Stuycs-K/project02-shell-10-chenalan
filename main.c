#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"
#include "exec.h"
#include "input.h"

int main(int argc, char *argv[]) {
    while (1) {
        char user_input[512];

        char *wd = get_wd_absolute();
        replace_homedir_in_path(wd);
        printf("%s$ ", wd);
        fflush(stdout);

        fgets(user_input, sizeof(user_input), stdin);
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
