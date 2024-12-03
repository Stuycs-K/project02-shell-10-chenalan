#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

/*
    Used internally to print the corresponding error message when execvp fails,
    most commonly when the command doesn't exist.

    PARAMS
        int exit_status: The status of the exiting child process that waitpid wrote to.
            Child processes will exit with errno if execvp fails.

    RETURNS
        None
*/
void print_execvp_error(int exit_status) {
    int exit_value = WEXITSTATUS(exit_status);

    if (exit_value == 0) { // We're fine
        return;
    } else { // exit_value is errno
        printf("%s\n", strerror(exit_value));
    }
}

/*
    Forks to run a command with execvp, for which the parent process waits.

    PARAMS
        char **args: The array of command arguments. Unused for parent process.

    RETURNS
        ?
*/
int exec(char **args) {
    pid_t pid = fork();

    int status = 0;

    if (pid == 0) {
        int result = execvp(args[0], args);
        if (result == -1) {
            exit(errno);
        } else {
            exit(0);
        }
    } else if (pid > 0) {
        waitpid(pid, &status, 0);

        print_execvp_error(status);
    }

    return 0;
}