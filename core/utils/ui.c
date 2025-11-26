#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

static int ui_go_root = 0;

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
    int len = (int)strlen(title);
    int width = len + 8;
    int inner;
    int i;
    int pad;
    int right;

    if (width < 40) width = 40;
    inner = width - 2;

    printf("\n");
    putchar('+');
    for (i = 0; i < inner; i++) putchar('-');
    putchar('+');
    putchar('\n');

    putchar('|');
    pad = (inner - len) / 2;
    right = inner - len - pad;
    for (i = 0; i < pad; i++) putchar(' ');
    fputs(title, stdout);
    for (i = 0; i < right; i++) putchar(' ');
    putchar('|');
    putchar('\n');

    putchar('+');
    for (i = 0; i < inner; i++) putchar('-');
    putchar('+');
    putchar('\n');
    putchar('\n');
}

int ui_line_is_back(const char* s) {
    if (!s) return 0;
    if (s[0] == '0') {
        ui_go_root = 1;
        return 1;
    }
    return 0;
}

void ui_set_go_root() {
    ui_go_root = 1;
}

int ui_should_go_root() {
    return ui_go_root;
}

void ui_reset_go_root() {
    ui_go_root = 0;
}
