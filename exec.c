#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cd.h"
#include "exec.h"
#include "parse.h"
#include "redirection.h"

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
    Errors are outputted to the shell with printf (will change).

    PARAMS
        char **args: The array of command arguments. Unused for parent process.

    RETURNS
        The parent process returns the child process's exit status.
*/
int run_process(char **args) {
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
        pid_t child_pid = waitpid(pid, &status, 0);

        print_execvp_error(status);

        return status;
    } else {
        perror("[exec]: Fork error");
        return -1;
    }
}

/*
    Runs the command. If the command is built-in, such as "cd," we directly
    call the related function. Otherwise, we fork a new process and use execvp.

    PARAMS
        char **args: The array of command arguments.

    RETURNS
        The exit status of the child process that ran the command.
*/
int exec(char **args) {
    char *command = args[0];

    // Do nothing if there's no command! This is so simply pressing enter doesn't output an error message.
    if (command[0] == 0) {
        return 0;
    }

    // Run our built-in commands here. Don't fork for any of them.
    if (!strcmp(command, "cd")) {
        cd(args);
        return 0;
    } else if (!strcmp(command, "exit")) {
        exit(0);
    }

    return run_process(args);
}
