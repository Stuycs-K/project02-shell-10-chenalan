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
* Don't expect good error handling

## Function Headers
