#ifndef PARSE_H
#define PARSE_H

void parse_command_args(char *line, char **arg_array);

void parse_commands(char *line, char **command_array);

char *format_line(char *line);

void separate_tokens(char *line, char **tokens);

CommandChain **build_command_chains(char **tokens);

#endif
