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

// To make the pipes easier to read
#define WRITE_END 1
#define READ_END 0

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
        waitpid(pid, &status, 0);
        return 0;
    } else if (pid < 0) {
        perror("[exec]: Fork error");
        return -1;
    }
}

/*
    Executes one command. If the command is built-in, such as "cd," we directly
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

/*
    Runs one command chain. stdin is redirected initially, if instructed. Each
    command pipes its output to the subsequent command. When we reach the last
    command in the chain, we redirect stdout, if instructed.

    PARAMS
        CommandChain *chain: The command chain.

    RETURNS
        The exit status of the child process that ran the command.
*/
int exec_chain(CommandChain *chain) {
    // All redirection is done before forking because the children will inherit the redirected files

    // Save normal stdin and stdout
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);

    int in_fd;
    int out_fd;

    // First, set in_fd to the replacement stdin, if specified.
    char *in_file = chain->in_file;
    if (in_file) {
        in_fd = open(in_file, O_RDONLY);
    } else {
        in_fd = dup(stdin_copy);
    }

    int pipe_fds[2];

    for (int i = 0; i < chain->command_count; ++i) {
        Command *command = chain->commands[i];

        // If we don't close our descriptors as soon as possible, dup2 hangs FOREVER!
        // First redirect input
        if (in_fd > -1) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        // This is the last command, so redirect stdout if possible
        if (i == chain->command_count - 1) {
            char *out_file = chain->out_file;
            if (out_file) {
                out_fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            } else {
                out_fd = dup(stdout_copy);
            }
        } else {
            pipe(pipe_fds);

            // Output goes to the write end of the pipe
            out_fd = pipe_fds[WRITE_END];
            // Set in_fd so the next command can read that output from the read end
            in_fd = pipe_fds[READ_END];
        }

        // Set stdout. If piped, this is the pipe write end. If not, it's the out file.
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        exec(command->args);
    }

    // Restore stdin and stdout
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
}
