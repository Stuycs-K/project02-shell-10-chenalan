#ifndef PARSE_H
#define PARSE_H

char *format_line(char *line);

void separate_tokens(char *line, char **tokens);

CommandChain **build_command_chains(char **tokens);

#endif
