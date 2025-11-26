#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "menu.h"
#include "shell/shell.h"
#include "http/http.h"
#include "db/db.h"
#include "monitor/monitor.h"
#include "tools/tools.h"

void menu_principal() {
    int opcao;
    char line[32];

    while (1) {
        ui_clear();
        ui_header("C - SYSTEMSUITE MENU");

        printf("[1] Shell\n");
        printf("[2] Servidor HTTP\n");
        printf("[3] Monitor de Processos\n");
        printf("[4] Banco de Dados Mini-SQL\n");
        printf("[5] Ferramentas\n");
        printf("[0] Sair\n");
        printf("Opcao: ");

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        if (line[0] == '0') {
            break;
        }

        opcao = atoi(line);

        if (opcao == 1) {
            ui_reset_go_root();
            shell_main();
        } else if (opcao == 2) {
            ui_reset_go_root();
            http_main();
        } else if (opcao == 3) {
            ui_reset_go_root();
            monitor_main();
        } else if (opcao == 4) {
            ui_reset_go_root();
            db_main();
        } else if (opcao == 5) {
            ui_reset_go_root();
            tools_main();
        }
    }
}
