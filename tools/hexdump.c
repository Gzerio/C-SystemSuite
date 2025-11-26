#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "tools.h"

static uint16_t rd16(const unsigned char* p) {
    return (uint16_t)(p[0] | (p[1] << 8));
}

static uint32_t rd32(const unsigned char* p) {
    return (uint32_t)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
}

void tools_hexdump() {
    char path[256];
    FILE* f;
    unsigned char buffer[16];
    size_t lidos;
    unsigned long offset = 0;

    printf("Arquivo para hexdump: ");
    if (fgets(path, sizeof(path), stdin) == NULL) {
        return;
    }

    path[strcspn(path, "\n")] = 0;

    if (path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    f = fopen(path, "rb");
    if (!f) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }

    unsigned char header[1024];
    size_t header_lidos = fread(header, 1, sizeof(header), f);

    if (header_lidos >= 64 && header[0] == 'M' && header[1] == 'Z') {
        uint32_t e_lfanew = rd32(header + 0x3C);
        int eh_pe = 0;
        if (e_lfanew + 24 < header_lidos) {
            if (header[e_lfanew] == 'P' &&
                header[e_lfanew + 1] == 'E' &&
                header[e_lfanew + 2] == 0 &&
                header[e_lfanew + 3] == 0) {
                eh_pe = 1;
            }
        }

        if (eh_pe) {
            const unsigned char* pe = header + e_lfanew;
            uint16_t machine = rd16(pe + 4);
            uint16_t num_sections = rd16(pe + 6);
            uint16_t characteristics = rd16(pe + 22);
            const unsigned char* opt = pe + 24;
            uint16_t magic = rd16(opt + 0);
            int is_dll = (characteristics & 0x2000) != 0;

            printf("PE detectado.\n");
            printf("Machine: 0x%04X", machine);
            if (machine == 0x14c) {
                printf(" (x86)");
            } else if (machine == 0x8664) {
                printf(" (x64)");
            }
            printf("\n");
            printf("Numero de secoes: %u\n", (unsigned int)num_sections);
            printf("Magic: 0x%04X", magic);
            if (magic == 0x10B) {
                printf(" (PE32)");
            } else if (magic == 0x20B) {
                printf(" (PE32+)");
            }
            printf("\n");
            printf("DLL flag: %s\n", is_dll ? "sim" : "nao");
        } else {
            printf("Assinatura MZ encontrada, mas PE nao localizado dentro do header lido.\n");
        }
    } else {
        printf("Assinatura MZ nao encontrada. Pode nao ser PE/DLL.\n");
    }

    fseek(f, 0, SEEK_SET);
    offset = 0;

    while ((lidos = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        printf("%08lX  ", offset);

        for (size_t i = 0; i < 16; i++) {
            if (i < lidos) {
                printf("%02X ", buffer[i]);
            } else {
                printf("   ");
            }
            if (i == 7) {
                printf(" ");
            }
        }

        printf(" ");

        for (size_t i = 0; i < lidos; i++) {
            if (isprint(buffer[i])) {
                printf("%c", buffer[i]);
            } else {
                printf(".");
            }
        }

        printf("\n");
        offset += lidos;

        if (offset >= 512) {
            break;
        }
    }

    fclose(f);
}
