#ifndef INPUT_H
#define INPUT_H

void strip_newline(char *string);

void parse_command_args(char *line, char **arg_array);

void parse_commands(char *line, char **command_array);

#endif
