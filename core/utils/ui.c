#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

void ui_clear() {
#ifdef _WIN32
    system("cls");
#else
    printf("\x1b[2J\x1b[H");
#endif
}

void ui_wait_enter() {
    int c;
    printf("\nPressione ENTER para continuar...");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void ui_header(const char* title) {
    printf("========================================\n");
    printf("%s\n", title);
    printf("========================================\n\n");
}
