#ifndef PARSE_H
#define PARSE_H

void parse_command_args(char *line, char **arg_array);

void parse_commands(char *line, char **command_array);

char *parse_stdin_redirect(char **arg_array);

char *parse_stdout_redirect(char **arg_array);

#endif
