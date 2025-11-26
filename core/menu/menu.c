#include <stdio.h>
#include "menu.h"

#include "../../shell/shell.h"
#include "../../http/http.h"
#include "../../db/db.h"
#include "../../monitor/monitor.h"
#include "../../tools/tools.h"

void menu_principal() {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n");
        printf("========================================\n");
        printf("          C - SYSTEMSUITE MENU          \n");
        printf("========================================\n");
        printf("[1] Shell\n");
        printf("[2] Servidor HTTP\n");
        printf("[3] Monitor de Processos\n");
        printf("[4] Banco de Dados Mini-SQL\n");
        printf("[5] Ferramentas\n");
        printf("[0] Sair\n");
        printf("----------------------------------------\n");
        printf("Sua opção: ");

        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: shell_main(); break;
            case 2: http_main(); break;
            case 3: monitor_main(); break;
            case 4: db_main(); break;
            case 5: tools_main(); break;
        }
    }
}
