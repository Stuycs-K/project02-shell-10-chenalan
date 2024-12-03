#ifndef CD_H
#define CD_H

char *get_wd_absolute(void);

char *get_homedir(void);

void shorten_homedir_in_path(char *wd);

int cd(char **argv);

#endif