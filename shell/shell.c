#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#define HISTORY_MAX 50

static char history[HISTORY_MAX][256];
static int history_count = 0;

static void add_history(const char* linha) {
    if (linha[0] == 0) {
        return;
    }
    int index = history_count % HISTORY_MAX;
    strncpy(history[index], linha, sizeof(history[index]) - 1);
    history[index][sizeof(history[index]) - 1] = 0;
    history_count++;
}

static void print_history() {
    int start = 0;
    int total = history_count;
    if (total > HISTORY_MAX) {
        start = total - HISTORY_MAX;
    }
    for (int i = start; i < total; i++) {
        int index = i % HISTORY_MAX;
        printf("%d  %s\n", i + 1, history[index]);
    }
}

static void print_prompt() {
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("csuite:%s> ", cwd);
    } else {
        printf("csuite> ");
    }
}

void shell_main() {
    char linha[256];
    char* argv[16];

    while (1) {
        print_prompt();

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            break;
        }

        linha[strcspn(linha, "\n")] = 0;

        if (linha[0] == 0) {
            continue;
        }

        if (strcmp(linha, "exit") == 0 ||
            strcmp(linha, "quit") == 0 ||
            strcmp(linha, "sair") == 0) {
            break;
        }

        if (strcmp(linha, "history") == 0) {
            print_history();
            continue;
        }

        if (strcmp(linha, "!!") == 0) {
            if (history_count == 0) {
                printf("Nenhum comando no historico.\n");
                continue;
            }
            int last_index = (history_count - 1) % HISTORY_MAX;
            strncpy(linha, history[last_index], sizeof(linha) - 1);
            linha[sizeof(linha) - 1] = 0;
            printf("%s\n", linha);
        }

        add_history(linha);

        int argc = shell_parse(linha, argv, 16);
        if (argc == 0) {
            continue;
        }

        shell_execute(argc, argv);
    }
}
