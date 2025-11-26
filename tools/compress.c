#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tools.h"

void tools_compress() {
    char in_path[256];
    char out_path[256];
    FILE* in;
    FILE* out;
    int c;
    int last = -1;
    uint8_t count = 0;

    printf("Arquivo de entrada: ");
    if (fgets(in_path, sizeof(in_path), stdin) == NULL) {
        return;
    }
    in_path[strcspn(in_path, "\n")] = 0;

    printf("Arquivo de saida: ");
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

    while ((c = fgetc(in)) != EOF) {
        if (last == -1) {
            last = c;
            count = 1;
        } else if (c == last && count < 255) {
            count++;
        } else {
            fputc(count, out);
            fputc(last, out);
            last = c;
            count = 1;
        }
    }

    if (last != -1 && count > 0) {
        fputc(count, out);
        fputc(last, out);
    }

    fclose(in);
    fclose(out);

    printf("Compressao concluida.\n");
}
