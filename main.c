#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "input.h"

int main() {
    char *cmdargv[256];
    char command[] = "cowsay -f calvin hey boys";
    parse_command_args(command, cmdargv);

    execvp(cmdargv[0], cmdargv);
    return 0;
}
