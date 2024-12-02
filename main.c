#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "exec.h"
#include "input.h"

int main(int argc, char *argv[]) {
    while (1) {
        char user_input[512];
        printf("Command: $ ");

        fgets(user_input, sizeof(user_input), stdin);
        strip_newline(user_input);

        char *commands[128];
        parse_commands(user_input, commands);

        char **current_command = commands;
        while (*current_command) {
            printf("%s\n", *current_command);
            current_command++;
        }
    }

    return 0;
}
