#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "ui.h"

void tools_main() {
    int opcao = -1;
    char line[32];

    while (!ui_should_go_root()) {
        ui_clear();
        ui_header("FERRAMENTAS");

        printf("[1] Calcular hash de arquivo\n");
        printf("[2] Comparar hash de dois arquivos\n");
        printf("[3] Comprimir arquivo (RLE)\n");
        printf("[4] Descomprimir arquivo (RLE)\n");
        printf("[5] Informacoes de arquivo\n");
        printf("[6] Hexdump de arquivo\n");
        printf("[7] Strings de arquivo\n");
        printf("[8] Decoder (hex / base64)\n");
        printf("[9] Scanner de array de bytes\n");
        printf("[0] Voltar\n");
        printf("Opcao: ");

        if (!fgets(line, sizeof(line), stdin)) {
            return;
        }

        if (ui_line_is_back(line)) {
            return;
        }

        opcao = atoi(line);

        if (opcao == 1) {
            ui_clear();
            ui_header("HASH DE ARQUIVO");
            tools_hash();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 2) {
            ui_clear();
            ui_header("COMPARAR HASH");
            tools_compare();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 3) {
            ui_clear();
            ui_header("COMPRIMIR ARQUIVO");
            tools_compress();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 4) {
            ui_clear();
            ui_header("DESCOMPRIMIR ARQUIVO");
            tools_decompress();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 5) {
            ui_clear();
            ui_header("INFO DE ARQUIVO");
            tools_fileinfo();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 6) {
            ui_clear();
            ui_header("HEXDUMP");
            tools_hexdump();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 7) {
            ui_clear();
            ui_header("STRINGS");
            tools_strings();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        } else if (opcao == 8) {
            ui_clear();
            ui_header("DECODER");
            tools_decode();
            if (ui_should_go_root()) return;
            ui_wait_enter();
        }else if (opcao == 9) {
    ui_clear();
    ui_header("SCANNER DE ARRAY DE BYTES");
    array_scan();
    if (ui_should_go_root()) return;
    ui_wait_enter();
}

    }
}
