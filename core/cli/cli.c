#include <stdio.h>
#include <stdlib.h>
#include "cli.h"

void cli_clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void cli_pause() {
    printf("Pressione ENTER para continuar...");
    getchar();
}
