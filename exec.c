#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec.h"

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
            exit(-1);
        }
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
    }

    return 0;
}