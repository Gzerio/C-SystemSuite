#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "../core/cli/cli.h"

#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

void shell_execute(int argc, char** argv) {
    if (argc == 0) {
        return;
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("Comandos internos:\n");
        printf("  cd <dir>      - muda o diretorio atual\n");
        printf("  clear / cls   - limpa a tela\n");
        printf("  pause         - pausa ate pressionar ENTER\n");
        printf("  history       - mostra historico de comandos\n");
        printf("  !!            - repete o ultimo comando\n");
        printf("  help          - mostra esta ajuda\n");
        printf("  csinfo        - mostra informacoes do sistema\n");
        printf("  exit/quit/sair- sai do shell\n");
        return;
    }

    if (strcmp(argv[0], "csinfo") == 0) {
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Diretorio atual: %s\n", cwd);
        }
#ifdef _WIN32
        printf("Sistema: Windows\n");
#else
        printf("Sistema: Unix-like\n");
#endif
        return;
    }

    if (strcmp(argv[0], "cd") == 0) {
        if (argc < 2) {
            printf("Uso: cd <diretorio>\n");
            return;
        }

        if (chdir(argv[1]) != 0) {
            perror("cd");
        }

        return;
    }

    if (strcmp(argv[0], "clear") == 0 || strcmp(argv[0], "cls") == 0) {
        cli_clear();
        return;
    }

    if (strcmp(argv[0], "pause") == 0) {
        cli_pause();
        return;
    }

    char comando[256];
    comando[0] = 0;
    int i;
    size_t offset = 0;

    for (i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (offset + len + 2 >= sizeof(comando)) {
            break;
        }

        memcpy(comando + offset, argv[i], len);
        offset += len;

        if (i < argc - 1) {
            comando[offset] = ' ';
            offset++;
        }
    }

    comando[offset] = 0;

    if (comando[0] == 0) {
        return;
    }

    system(comando);
}
