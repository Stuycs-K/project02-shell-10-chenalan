#ifndef SHELL_H
#define SHELL_H

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define SHELL_STATUS_EXEC 1
#define SHELL_STATUS_IDLE 0

void output_prompt(void);

void shell_loop(void);

extern int shell_status;

#endif
