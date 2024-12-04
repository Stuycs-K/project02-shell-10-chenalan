#ifndef SHELL_H
#define SHELL_H

#define SHELL_STATUS_EXEC 1
#define SHELL_STATUS_IDLE 0

void output_prompt();

void shell_loop();

extern int shell_status;

#endif
