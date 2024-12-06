[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell

**"Team" Name**: Sergeant of the Master Sergeants Most Important Person of Extreme Sergeants to the Max!

**Member**: Alan Chen

## Features
In addition to all required features:
* Home directory in prompt replaced with `~`
* `cd` expands to `cd ~`, and `~` will expand to the home directory
* Commands can be separated with any number of whitespace characters (e.g. `ls ; sleep  1; ls` is valid)
* Sending `SIGINT`, `SIGQUIT`, or `SIGTSTP` will only affect child processes; the shell will continue running
* Works to a reasonable degree with multiple pipes (e.g. `ls | wc -l | cowsay`)
* Supports multiple redirect operators (`<` and `>`), in any order (e.g. `wc < shell.h > output.txt < shell.c`)
    * Redirect operators are read left-to-right; if there's multiple of the same redirect operator, only the file following the right-most operator will be used as the redirect
    * Behavior differs from bash in that intermediate stdout redirect files will not be created. For example, in bash, the command `ls > f1 > f2` will create files `f1` and `f2`. `f1` will be empty, and `f2` will contain the output of `ls`. Here, the same command will only create and redirect stdout into `f2`

## Notes
* Don't expect amazing error handling

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
typedef struct {
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
typedef struct {
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
