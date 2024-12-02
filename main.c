#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "input.h"

int main(int argc, char *argv[]) {
    char user_input[512];
    printf("Command: ");

    fgets(user_input, sizeof(user_input), stdin);
    strip_newline(user_input);

    printf("%s\n", user_input);

    char *commands[128];
    parse_commands(user_input, commands);

    char **ptr = commands;
    while (*ptr) {
        printf("%s\n", *ptr);
        ptr++;
    }

    return 0;
}
