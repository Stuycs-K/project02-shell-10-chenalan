#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "input.h"

int main(int argc, char *argv[]) {
    char user_input[512];
    printf("Command: ");

    fgets(user_input, sizeof(user_input), stdin);

    // TODO: Split with semicolons first!

    return 0;
}
