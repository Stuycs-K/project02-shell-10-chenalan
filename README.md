[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell

**"Team" Name**: Sergeant of the Master Sergeants Most Important Person of Extreme Sergeants to the Max!

**Member**: Alan Chen

## Features
In addition to all required features:
* Home directory in prompt replaced with `~`
* `cd` expands to `cd ~`, and `~` expands to the home directory
* Commands can be separated with any number of whitespace characters (e.g. `ls ; sleep  1; ls` is valid)
* Sending `SIGINT`, `SIGQUIT`, or `SIGTSTP` will only affect child processes; the shell will continue running
* Works to a reasonable degree with multiple pipes (e.g. `ls | wc -l | cowsay`)
* Supports multiple redirect operators (`<` and `>`), in any order (e.g. `wc < shell.h > output.txt < shell.c`)
    * Redirect operators are read left-to-right; if there's multiple of the same redirect operator, only the file following the right-most operator will be used as the redirect
    * Behavior differs from bash in that intermediate stdout redirect files will not be created. For example, in bash, the command `ls > f1 > f2` will create files `f1` and `f2`. `f1` will be empty, and `f2` will contain the output of `ls`. Here, the same command will only create and redirect stdout into `f2`
* Supports command arguments in quotations (e.g. `cat "spaced out"` or `cat 'spaced out'`)

## Unimplemented Features
* Supporting more operators such as `&&` and `||`

## Potential Issues
* Don't expect amazing error handling
* Because the shell uses temporary files to handle piping, `wc` can estimate column widths from the temporary pipe file's metadata. Thus, its output does not use maximum column space padding. As a result, `echo "hello hello" | wc` will output ` 1  2 12` instead of `      1       2      12`

## Documentation

### `command.h`
#### Structs
```c

/*
    Command holds arguments for ONE command (e.g. ls -al)

    char **args: The argument array.
    int arg_count: The current number of arguments. Used for resizing.
    int args_size: The current maximum number of char * in args. Used for resizing.
*/
typedef struct Command {
    char **args;
    int arg_count;
    int args_size;
} Command;

/*
    CommandChain holds a sequence of commands. The output from each of its commands
    will be piped into the subsequent command.

    The first command's input will be from in_file (default: stdin).
    The last command's output will go to out_file (default: stdout).

    Command **commands: The command array.
    int command_count: The current number of commands. Used for resizing.
    int commands_size: The current maximum number of Command * in commands. Used for resizing.

    char *in_file: stdin will redirect to this. NULL (or stdin) by default.
    char *out_file: stdout will redirect to this. NULL (or stdout) by default.
*/
typedef struct CommandChain {
    Command **commands;
    int command_count;
    int commands_size;

    char *in_file;
    char *out_file;
} CommandChain;

```
#### Functions
```c

/*
    Constructs a new command. Initializes its argument array.

    PARAMS
        None.

    RETURNS
        The created command.
*/
Command *new_command();

/*
    Inserts an argument into the command's argument array.
    Resizes if necessary.

    PARAMS
        Command *command: The command.
        char *arg: The argument to insert.

    RETURNS
        None
*/
void insert_arg(Command *command, char *arg);

/*
    Sets the command chain's redirect file for stdin. If a redirect file is
    already set, it is overridden.

    PARAMS
        CommandChain *chain: The command chain.
        char *file_name: A string containing the redirect file. Will be copied.

    RETURNS
        None.
*/
void set_in_file(CommandChain *command, char *file_name);

/*
    Sets the command chain's redirect file for stdout. If a redirect file is
    already set, it is overridden.

    PARAMS
        CommandChain *chain: The command chain.
        char *file_name: A string containing the redirect file. Will be copied.

    RETURNS
        None.
*/
void set_out_file(CommandChain *command, char *file_name);

/*
    Cleans up the command.

    PARAMS
        Command *command: The command.

    RETURNS
        NULL.
*/
Command *free_command(Command *command);

/*
    Constructs a new command chain. Initializes its commands array.

    PARAMS
        None.

    RETURNS
        The new command chain.
*/
CommandChain *new_command_chain();

/*
    Inserts a command into the chain's command array. Resizes if necessary.

    PARAMS
        CommandChain *chain: The command chain.
        Command *command: The command to insert.

    RETURNS
        None.
*/
void insert_command(CommandChain *chain, Command *command);

/*
    Cleans up the command chain and calls free_command on all its commands.

    PARAMS
        CommandChain *chain: The command chain.

    RETURNS
        NULL.
*/
CommandChain *free_command_chain(CommandChain *chain);

```

### `parse.h`
#### Functions
```c

/*
    Adds spaces between special characters in the command line for easy separation.
    The special characters are |, <, >, and ;

    PARAMS
        char *line: The input line.

    RETURNS
        A modified heap string, with whitespace between each found special character.
        Must be freed.
*/
char *format_line(char *line);

/*
    Inserts whitespace-separated tokens from an input line into an array.
    The tokens can be separated with any number of spaces.

    PARAMS
        char *line: The input line.

        char **tokens: The array into which each token will be inserted.

    RETURNS
        None.
*/
void separate_tokens(char *line, char **tokens);

/*
    The main parse function. Builds a list of command chains from a token list.
    If we encounter ";", begin building a new command chain.
    If we encounter "|", set a flag that the next word will go towards a new command.
    If we encounter "<" or ">", set the input and output files of the current command chain.

    Any other word is inserted to the current command's argument list.

    PARAMS
        char **tokens: The tokens.

    RETURNS
        An null-terminated array of command chains, each of which can be executed.
        Must be freed.
*/
CommandChain **build_command_chains(char **tokens);

```
### `exec.h`
#### Functions
```c

/*
    Forks to run a command with execvp, for which the parent process waits.

    PARAMS
        char **args: The array of command arguments. Unused for parent process.

    RETURNS
        None.
*/
void run_process(char **args);

/*
    Executes one command. If the command is built-in, such as "cd," we directly
    call the related function. Otherwise, we fork a new process and use execvp.

    PARAMS
        char **args: The array of command arguments.

    RETURNS
        None.
*/
void exec(char **args);

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
void exec_chain(CommandChain *chain);

```

### `cd.h`
#### Functions
```c

/*
    Returns a string containing the absolute path to the working directory

    PARAMS
        None

    RETURNS
        A heap string with the absolute path. Must be freed.
*/
char *get_wd_absolute(void);

/*
    Returns a string containing the home directory.
    Source: https://stackoverflow.com/questions/2910377/get-home-directory-in-linux

    PARAMS
        None

    RETURNS
        A heap string with the home directory. Must be freed.
*/
char *get_homedir(void);

/*
    Shortens a path by replacing the home directory (if found) with "~".

    PARAMS
        char *wd: The buffer containing the absolute path. Will be modified.

    RETURNS
        None. There's no need to create a new buffer, as path will
        have enough space for the shortened path.
*/
void shorten_homedir_in_path(char *path);

/*
    Used internally. Expands "~" in a path by replacing it with the home directory.

    PARAMS
        char *path: The buffer containing the path relative to the home directory.

    RETURNS
        A new heap buffer containing the expanded path. Must be freed.
        If the path does not contain "~", returns a copy of the path passed in.
*/
char *expand_homedir_in_path(char *path);

/*
    Runs the cd command. Changes the working directory to the one requested,
    returning errno if something goes wrong. If no path is specified,
    the path will be the home directory. "~" at the beginning of a path will be expanded
    into the home directory.

    PARAMS
        char **argv: The argument array normally passed into execvp. The first
            argument should be "cd," and the next could either contain NULL or
            a path.

    RETURNS
        None.
*/
void cd(char **argv);

```

### `shell.h`
#### Functions
```c

/*
    Prints the shell prompt. If stdin has been redirected, this prints nothing.
    The home directory, if present, is replaced with "~".

    PARAMS
        None.

    RETURNS
        None.
*/
void output_prompt(void);

/*
    Reads a line from stdin. If fgets encounters EOF, the shell terminates.
    The last newline is stripped.

    PARAMS
        None.

    RETURNS
        A heap string containing the read line. Must be freed.
*/
char *read_line(void);

/*
    Executes each command chain.

    PARAMS
        CommandChain **command_chains: The array of command chains.

    RETURNS
        None.
*/
void run_commands(CommandChain **command_chains);

/*
    Runs the main loop of the shell. Outputs the prompt, reads user input, parses,
    and runs commands.

    shell_status is set to EXEC before run_commands and back to IDLE after it resolves.

    PARAMS
        None.

    RETURNS
        None.
*/
void shell_loop(void);

```

### `main.c`
#### Functions
```c

/*
    Prevents the shell from terminating on SIGINT, SIGQUIT, and SIGTSTP.
    Outputs a new prompt if no child processes are currently running (this is race condition
    unsafe).

    PARAMS
        int signo: The signal number.

    RETURN
        None
*/
static void ignore_signal(int signo);

```