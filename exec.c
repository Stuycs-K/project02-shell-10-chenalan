#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cd.h"
#include "command.h"
#include "exec.h"
#include "parse.h"

#define TEMP_PIPE_FILE "__temp__"

/*
    Forks to run a command with execvp, for which the parent process waits.

    PARAMS
        char **args: The array of command arguments. Unused for parent process.

    RETURNS
        None.
*/
void run_process(char **args) {
    pid_t pid = fork();

    int status = 0;

    if (pid == 0) {
        int result = execvp(args[0], args);
        if (result == -1) {

            // Command not found
            if (errno == ENOENT) {
                fprintf(stderr, "%s: command not found\n", args[0]);
            }

            exit(errno);
        } else {
            exit(0);
        }
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
    } else if (pid < 0) {
        perror("[exec]: Fork error");
    }
}

/*
    Executes one command. If the command is built-in, such as "cd," we directly
    call the related function. Otherwise, we fork a new process and use execvp.

    PARAMS
        char **args: The array of command arguments.

    RETURNS
        None.
*/
void exec(char **args) {
    char *command = args[0];

    // Note: This condition should never be met.
    // Do nothing if there's no command! This is so simply pressing enter doesn't output an error message.
    if (command[0] == 0) {
        return;
    }

    // Run our built-in commands here. Don't fork for any of them.
    if (!strcmp(command, "cd")) {
        cd(args);
        return;
    } else if (!strcmp(command, "exit")) {
        exit(0);
    }

    run_process(args);
}

/*
    Runs one command chain. stdin is redirected initially, if instructed. Each
    command pipes its output to the subsequent command. When we reach the last
    command in the chain, we redirect stdout, if instructed.

    All redirection is done before forking because the children will inherit the redirected files.

    PARAMS
        CommandChain *chain: The command chain.

    RETURNS
        None.
*/
void exec_chain(CommandChain *chain) {
    // Save normal stdin and stdout
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);

    int in_fd;
    int out_fd;

    // First, set in_fd to the replacement stdin, if specified.
    if (chain->in_file) {
        in_fd = open(chain->in_file, O_RDONLY);
    } else {
        in_fd = dup(stdin_copy);
    }

    for (int i = 0; i < chain->command_count; ++i) {
        Command *command = chain->commands[i];

        // First redirect input
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);

        // This is the last command, so redirect stdout if possible
        if (i == chain->command_count - 1) {
            if (chain->out_file) {
                out_fd = open(chain->out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            } else {
                out_fd = dup(stdout_copy);
            }
        } else {
            // Open the temp file to WRITE (pipe write end)
            out_fd = open(TEMP_PIPE_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);

            // Open the same file to READ (pipe read end). The next iteration will use this as its input.
            in_fd = open(TEMP_PIPE_FILE, O_RDONLY);

            // Remove the file immediately from the directory. Apparently, we can still
            // read and write to the file, but it won't appear in ls
            remove(TEMP_PIPE_FILE);
        }

        // Set stdout. If piped, this is the pipe write end. If not, it's the out file.
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        exec(command->args);
    }

    // Restore stdin and stdout
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);

    close(stdin_copy);
    close(stdout_copy);
}
