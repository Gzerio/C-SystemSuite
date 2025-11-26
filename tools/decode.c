#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "tools.h"

static int hex_val(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static void decode_hex() {
    char input[1024];
    unsigned char out[512];
    int len_in;
    int i;
    int oi = 0;

    printf("Hex para decodificar (sem 0x, so caracteres hex): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }

    input[strcspn(input, "\n")] = 0;
    len_in = (int)strlen(input);

    if (len_in < 2) {
        printf("Entrada muito curta.\n");
        return;
    }

    if (len_in % 2 != 0) {
        printf("Tamanho de hex invalido (precisa ser par).\n");
        return;
    }

    for (i = 0; i < len_in; i += 2) {
        int h1 = hex_val(input[i]);
        int h2 = hex_val(input[i + 1]);
        if (h1 < 0 || h2 < 0) {
            printf("Caractere nao-hex encontrado.\n");
            return;
        }
        out[oi++] = (unsigned char)((h1 << 4) | h2);
        if (oi >= (int)sizeof(out)) {
            break;
        }
    }

    printf("Bytes decodificados (%d bytes):\n", oi);
    for (i = 0; i < oi; i++) {
        if (isprint(out[i])) {
            printf("%c", out[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

static int base64_val(int c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return -2;
    return -1;
}

static void decode_base64() {
    char input[2048];
    unsigned char out[1024];
    int len_in;
    int i = 0;
    int oi = 0;

    printf("Base64 para decodificar: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }

    input[strcspn(input, "\n")] = 0;
    len_in = (int)strlen(input);

    if (len_in < 4) {
        printf("Entrada muito curta.\n");
        return;
    }

    while (i + 3 < len_in) {
        int v1 = base64_val(input[i]);
        int v2 = base64_val(input[i + 1]);
        int v3 = base64_val(input[i + 2]);
        int v4 = base64_val(input[i + 3]);

        if (v1 < 0 || v2 < 0 || v3 < -1 || v4 < -1) {
            printf("Caractere invalido em base64.\n");
            return;
        }

        if (v3 == -2 && v4 == -2) {
            int b1 = (v1 << 2) | (v2 >> 4);
            out[oi++] = (unsigned char)b1;
            break;
        } else if (v3 >= 0 && v4 == -2) {
            int b1 = (v1 << 2) | (v2 >> 4);
            int b2 = ((v2 & 0xF) << 4) | (v3 >> 2);
            out[oi++] = (unsigned char)b1;
            out[oi++] = (unsigned char)b2;
            break;
        } else if (v3 >= 0 && v4 >= 0) {
            int b1 = (v1 << 2) | (v2 >> 4);
            int b2 = ((v2 & 0xF) << 4) | (v3 >> 2);
            int b3 = ((v3 & 0x3) << 6) | v4;
            out[oi++] = (unsigned char)b1;
            out[oi++] = (unsigned char)b2;
            out[oi++] = (unsigned char)b3;
        } else {
            break;
        }

        if (oi >= (int)sizeof(out)) {
            break;
        }

        i += 4;
    }

    printf("Bytes decodificados (%d bytes):\n", oi);
    for (i = 0; i < oi; i++) {
        if (isprint(out[i])) {
            printf("%c", out[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

void tools_decode() {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n");
        printf("==== DECODER ====\n");
        printf("[1] Hex -> texto\n");
        printf("[2] Base64 -> texto\n");
        printf("[0] Voltar\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
            continue;
        }

        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }

        if (opcao == 1) {
            decode_hex();
        } else if (opcao == 2) {
            decode_base64();
        }
    }
}
