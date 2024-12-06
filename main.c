#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "shell.h"

/*
    Prevents the shell from terminating on SIGINT, SIGQUIT, and SIGTSTP.
    Outputs a new prompt if no child processes are currently running (this is race condition
    unsafe).

    PARAMS
        int signo: The signal number.

    RETURN
        None
*/
static void ignore_signal(int signo) {
    printf("\n");

    if (shell_status == SHELL_STATUS_IDLE) {
        output_prompt();
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, ignore_signal);
    signal(SIGQUIT, ignore_signal);
    signal(SIGTSTP, ignore_signal);

    while (1) {
        shell_loop();
    }

    return 0;
}
