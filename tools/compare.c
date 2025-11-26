#include <stdio.h>
#include <string.h>
#include "tools.h"

void tools_compare() {
    char path1[256];
    char path2[256];
    int ok1;
    int ok2;
    unsigned int h1;
    unsigned int h2;

    printf("Primeiro arquivo: ");
    if (fgets(path1, sizeof(path1), stdin) == NULL) {
        return;
    }

    path1[strcspn(path1, "\n")] = 0;

    printf("Segundo arquivo: ");
    if (fgets(path2, sizeof(path2), stdin) == NULL) {
        return;
    }

    path2[strcspn(path2, "\n")] = 0;

    if (path1[0] == 0 || path2[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    h1 = tools_hash_file(path1, &ok1);
    h2 = tools_hash_file(path2, &ok2);

    if (!ok1 || !ok2) {
        if (!ok1) {
            printf("Nao foi possivel abrir o primeiro arquivo.\n");
        }
        if (!ok2) {
            printf("Nao foi possivel abrir o segundo arquivo.\n");
        }
        return;
    }

    printf("Hash 1: 0x%08X\n", h1);
    printf("Hash 2: 0x%08X\n", h2);

    if (h1 == h2) {
        printf("Arquivos iguais pelo hash.\n");
    } else {
        printf("Arquivos diferentes pelo hash.\n");
    }
}
