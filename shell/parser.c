#include <string.h>
#include "shell.h"

int shell_parse(char* linha, char** argv, int max_args) {
    int argc = 0;
    char* p = linha;

    while (*p != 0 && argc < max_args) {
        while (*p == ' ' || *p == '\t') {
            p++;
        }

        if (*p == 0) {
            break;
        }

        argv[argc++] = p;

        while (*p != 0 && *p != ' ' && *p != '\t') {
            p++;
        }

        if (*p == 0) {
            break;
        }

        *p = 0;
        p++;
    }

    return argc;
}
