#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tools.h"

static int eh_char_permitido(int c) {
    if (c >= 'A' && c <= 'Z') return 1;
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= '0' && c <= '9') return 1;
    if (c == ' ') return 1;
    if (c == '.') return 1;
    if (c == '_') return 1;
    if (c == '-') return 1;
    if (c == '/') return 1;
    if (c == ':') return 1;
    if (c == '\\') return 1;
    return 0;
}

static int tem_letra_ou_numero(const char* s, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if ((s[i] >= 'A' && s[i] <= 'Z') ||
            (s[i] >= 'a' && s[i] <= 'z') ||
            (s[i] >= '0' && s[i] <= '9')) {
            return 1;
        }
    }
    return 0;
}

void tools_strings() {
    char path[256];
    char out_path[256];
    char opt[16];
    char opt_filter[16];
    char filtro[128];
    FILE* f;
    FILE* out = NULL;
    int salvar = 0;
    int usar_filtro = 0;
    int c;
    char buf[512];
    int len = 0;
    int min_len = 8;

    printf("Arquivo para extrair strings: ");
    if (fgets(path, sizeof(path), stdin) == NULL) {
        return;
    }

    path[strcspn(path, "\n")] = 0;

    if (path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    printf("Salvar em arquivo? (s/n): ");
    if (fgets(opt, sizeof(opt), stdin) == NULL) {
        return;
    }

    if (opt[0] == 's' || opt[0] == 'S') {
        salvar = 1;
        printf("Arquivo de saida (.txt): ");
        if (fgets(out_path, sizeof(out_path), stdin) == NULL) {
            return;
        }
        out_path[strcspn(out_path, "\n")] = 0;
        if (out_path[0] == 0) {
            printf("Caminho vazio.\n");
            return;
        }
        out = fopen(out_path, "w");
        if (!out) {
            printf("Nao foi possivel criar o arquivo de saida.\n");
            return;
        }
    }

    printf("Filtrar por texto? (s/n): ");
    if (fgets(opt_filter, sizeof(opt_filter), stdin) == NULL) {
        if (out) fclose(out);
        return;
    }

    if (opt_filter[0] == 's' || opt_filter[0] == 'S') {
        usar_filtro = 1;
        printf("Texto para filtro: ");
        if (fgets(filtro, sizeof(filtro), stdin) == NULL) {
            if (out) fclose(out);
            return;
        }
        filtro[strcspn(filtro, "\n")] = 0;
        if (filtro[0] == 0) {
            usar_filtro = 0;
        }
    }

    f = fopen(path, "rb");
    if (!f) {
        if (out) fclose(out);
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }

    while ((c = fgetc(f)) != EOF) {
        if (eh_char_permitido(c)) {
            if (len < (int)(sizeof(buf) - 1)) {
                buf[len++] = (char)c;
            }
        } else {
            if (len >= min_len && tem_letra_ou_numero(buf, len)) {
                buf[len] = 0;
                if (usar_filtro) {
                    if (strstr(buf, filtro) == NULL) {
                        len = 0;
                        continue;
                    }
                }
                if (salvar && out) {
                    fprintf(out, "%s\n", buf);
                } else {
                    printf("%s\n", buf);
                }
            }
            len = 0;
        }
    }

    if (len >= min_len && tem_letra_ou_numero(buf, len)) {
        buf[len] = 0;
        if (!usar_filtro || strstr(buf, filtro) != NULL) {
            if (salvar && out) {
                fprintf(out, "%s\n", buf);
            } else {
                printf("%s\n", buf);
            }
        }
    }

    fclose(f);
    if (out) {
        fclose(out);
        printf("Strings salvas em: %s\n", out_path);
    }
}
