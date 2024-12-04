#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "shell.h"

static void ignore_shell_signal(int signo) {
    printf("\n");

    if (shell_status == SHELL_STATUS_IDLE) {
        output_prompt();
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, ignore_shell_signal);
    signal(SIGQUIT, ignore_shell_signal);

    while (1) {
        shell_loop();
    }

    return 0;
}
