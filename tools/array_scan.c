#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tools.h"
#include "ui.h"
#include "core/utils/logger.h"
#include "core/memory/allocator.h"

#define DEFAULT_BLOCK_SIZE 16
#define MIN_REPEATS 3
#define HASH_TABLE_SIZE 10000

typedef struct BlockEntry {
    unsigned char block[DEFAULT_BLOCK_SIZE];
    int count;
    struct BlockEntry* next;
} BlockEntry;

static BlockEntry* hash_table[HASH_TABLE_SIZE] = {0};

unsigned int fnv1a_hash(const unsigned char* data, size_t length) {
    unsigned int hash = 2166136261U;
    for (size_t i = 0; i < length; i++) {
        hash ^= data[i];
        hash *= 16777619U;
    }
    return hash;
}

void insert_block(const unsigned char* block) {
    unsigned int hash = fnv1a_hash(block, DEFAULT_BLOCK_SIZE);
    unsigned int index = hash % HASH_TABLE_SIZE;

    BlockEntry* entry = hash_table[index];
    while (entry) {
        if (memcmp(entry->block, block, DEFAULT_BLOCK_SIZE) == 0) {
            entry->count++;
            return;
        }
        entry = entry->next;
    }

    entry = (BlockEntry*)mem_alloc(sizeof(BlockEntry));
    memcpy(entry->block, block, DEFAULT_BLOCK_SIZE);
    entry->count = 1;
    entry->next = hash_table[index];
    hash_table[index] = entry;
}

void free_hash_table() {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        BlockEntry* entry = hash_table[i];
        while (entry) {
            BlockEntry* next = entry->next;
            mem_free(entry);
            entry = next;
        }
        hash_table[i] = NULL;
    }
}

static int contains_ignore_case(const char* haystack, const char* needle) {
    if (!needle || needle[0] == 0) return 1;
    size_t nlen = strlen(needle);
    size_t hlen = strlen(haystack);
    size_t i;
    size_t j;

    if (nlen == 0) return 1;
    if (nlen > hlen) return 0;

    for (i = 0; i + nlen <= hlen; i++) {
        j = 0;
        while (j < nlen) {
            unsigned char hc = (unsigned char)haystack[i + j];
            unsigned char nc = (unsigned char)needle[j];
            if (tolower(hc) != tolower(nc)) break;
            j++;
        }
        if (j == nlen) return 1;
    }
    return 0;
}

static int block_matches_filter(const unsigned char* block, const char* filter) {
    if (!filter || filter[0] == 0) return 1;

    char text[DEFAULT_BLOCK_SIZE + 1];
    int j;
    for (j = 0; j < DEFAULT_BLOCK_SIZE; j++) {
        unsigned char c = block[j];
        if (isprint(c)) {
            text[j] = (char)c;
        } else {
            text[j] = '.';
        }
    }
    text[DEFAULT_BLOCK_SIZE] = '\0';

    return contains_ignore_case(text, filter);
}

void scan_for_repeated_blocks(const char* filepath, const char* filter) {
    FILE* f = fopen(filepath, "rb");
    unsigned char buffer[DEFAULT_BLOCK_SIZE];
    int i;

    if (!f) {
        perror("Erro ao abrir arquivo");
        logger_error("array_scan falhou ao abrir arquivo %s", filepath);
        return;
    }

    logger_info("array_scan iniciado em %s filtro='%s'",
                filepath,
                filter && filter[0] ? filter : "");

    while (fread(buffer, 1, DEFAULT_BLOCK_SIZE, f) == DEFAULT_BLOCK_SIZE) {
        insert_block(buffer);
    }

    fclose(f);

    printf("Blocos repetidos (minimo %d repeticoes):\n\n", MIN_REPEATS);
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        BlockEntry* entry = hash_table[i];
        while (entry) {
            if (entry->count >= MIN_REPEATS && block_matches_filter(entry->block, filter)) {
                int j;
                printf("Padrao repetido %d vezes\n", entry->count);

                printf("Hex: ");
                for (j = 0; j < DEFAULT_BLOCK_SIZE; j++) {
                    printf("%02X ", entry->block[j]);
                }
                printf("\n");

                printf("String: ");
                for (j = 0; j < DEFAULT_BLOCK_SIZE; j++) {
                    unsigned char c = entry->block[j];
                    if (isprint(c)) {
                        putchar(c);
                    } else {
                        putchar('.');
                    }
                }
                printf("\n\n");

                entry->count = 0;
            }
            entry = entry->next;
        }
    }

    free_hash_table();
    logger_info("array_scan finalizado em %s", filepath);
}

void array_scan() {
    char path[256];
    char filter[128];

    printf("Arquivo para escanear blocos repetidos (0 para voltar): ");
    if (!fgets(path, sizeof(path), stdin)) {
        return;
    }

    path[strcspn(path, "\n")] = 0;

    if (ui_line_is_back(path)) {
        return;
    }

    if (path[0] == 0) {
        printf("Caminho vazio.\n");
        return;
    }

    printf("Filtro de string (ENTER para sem filtro): ");
    if (!fgets(filter, sizeof(filter), stdin)) {
        filter[0] = 0;
    } else {
        filter[strcspn(filter, "\n")] = 0;
        if (ui_line_is_back(filter)) {
            return;
        }
    }

    scan_for_repeated_blocks(path, filter);
}
