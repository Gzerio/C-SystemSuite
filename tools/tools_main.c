#include <stdio.h>
#include "tools.h"
#include "ui.h"

void tools_main() {
    int opcao = -1;

    while (opcao != 0) {
        ui_clear();
        ui_header("FERRAMENTAS");

        printf("[1] Calcular hash de arquivo\n");
        printf("[2] Comparar hash de dois arquivos\n");
        printf("[3] Comprimir arquivo (RLE)\n");
        printf("[4] Descomprimir arquivo (RLE)\n");
        printf("[5] Informacoes de arquivo\n");
        printf("[6] Hexdump de arquivo\n");
        printf("[7] Strings de arquivo\n");
        printf("[8] Decoder (hex / base64 / XOR)\n");
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
            ui_clear();
            ui_header("HASH DE ARQUIVO");
            tools_hash();
            ui_wait_enter();
        } else if (opcao == 2) {
            ui_clear();
            ui_header("COMPARAR HASH");
            tools_compare();
            ui_wait_enter();
        } else if (opcao == 3) {
            ui_clear();
            ui_header("COMPRIMIR ARQUIVO");
            tools_compress();
            ui_wait_enter();
        } else if (opcao == 4) {
            ui_clear();
            ui_header("DESCOMPRIMIR ARQUIVO");
            tools_decompress();
            ui_wait_enter();
        } else if (opcao == 5) {
            ui_clear();
            ui_header("INFO DE ARQUIVO");
            tools_fileinfo();
            ui_wait_enter();
        } else if (opcao == 6) {
            ui_clear();
            ui_header("HEXDUMP");
            tools_hexdump();
            ui_wait_enter();
        } else if (opcao == 7) {
            ui_clear();
            ui_header("STRINGS");
            tools_strings();
            ui_wait_enter();
        } else if (opcao == 8) {
            ui_clear();
            ui_header("DECODER");
            tools_decode();
            ui_wait_enter();
        }
    }
}
