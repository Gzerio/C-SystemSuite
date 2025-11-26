#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tools.h"

unsigned int tools_hash_file(const char* path, int* ok) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        if (ok) {
            *ok = 0;
        }
        return 0;
    }

    uint32_t hash = 2166136261u;
    unsigned char buffer[4096];
    size_t lidos;

    while ((lidos = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        for (size_t i = 0; i < lidos; i++) {
            hash ^= buffer[i];
            hash *= 16777619u;
        }
    }

    fclose(f);

    if (ok) {
        *ok = 1;
    }

    return (unsigned int)hash;
}

void tools_hash() {
    char path[256];
    int ok;
    unsigned int h;

    printf("Arquivo para hash: ");
    if (fgets(path, sizeof(path), stdin) == NULL) {
        return;
    }

    path[strcspn(path, "\n")] = 0;

    if (path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    h = tools_hash_file(path, &ok);
    if (!ok) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }

    printf("FNV-1a 32-bit: 0x%08X\n", h);
}
