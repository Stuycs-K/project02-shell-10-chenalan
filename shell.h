#ifndef SHELL_H
#define SHELL_H

void output_prompt();

char *read_line();

void run_commands(char **command_array);

void shell_loop();

#endif
