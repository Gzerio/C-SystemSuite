#include <stdio.h>
#include <string.h>
#include "tools.h"

#ifdef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#define stat _stat
#else
#include <sys/stat.h>
#endif

void tools_fileinfo() {
    char path[256];
    struct stat st;

    printf("Arquivo: ");
    if (fgets(path, sizeof(path), stdin) == NULL) {
        return;
    }

    path[strcspn(path, "\n")] = 0;

    if (path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    if (stat(path, &st) != 0) {
        printf("Nao foi possivel obter informacoes do arquivo.\n");
        return;
    }

    printf("Tamanho: %lld bytes\n", (long long)st.st_size);
}
