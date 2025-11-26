#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tools.h"

void tools_decompress() {
    char in_path[256];
    char out_path[256];
    FILE* in;
    FILE* out;
    int count;
    int value;

    printf("Arquivo comprimido (entrada): ");
    if (fgets(in_path, sizeof(in_path), stdin) == NULL) {
        return;
    }
    in_path[strcspn(in_path, "\n")] = 0;

    printf("Arquivo descomprimido (saida): ");
    if (fgets(out_path, sizeof(out_path), stdin) == NULL) {
        return;
    }
    out_path[strcspn(out_path, "\n")] = 0;

    if (in_path[0] == 0 || out_path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    in = fopen(in_path, "rb");
    if (!in) {
        printf("Nao foi possivel abrir o arquivo de entrada.\n");
        return;
    }

    out = fopen(out_path, "wb");
    if (!out) {
        fclose(in);
        printf("Nao foi possivel criar o arquivo de saida.\n");
        return;
    }

    while (1) {
        count = fgetc(in);
        if (count == EOF) {
            break;
        }

        value = fgetc(in);
        if (value == EOF) {
            break;
        }

        for (int i = 0; i < count; i++) {
            fputc(value, out);
        }
    }

    fclose(in);
    fclose(out);

    printf("Descompressao concluida.\n");
}
