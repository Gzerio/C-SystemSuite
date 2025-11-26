#include <stdio.h>
#include <string.h>
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

void monitor_process_overview() {
    ui_clear();
    ui_header("MONITOR - PROCESSOS");

#ifdef _WIN32
    HANDLE snap;
    PROCESSENTRY32 pe;
    int count = 0;

    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        printf("Falha ao criar snapshot de processos.\n");
        ui_wait_enter();
        return;
    }

    memset(&pe, 0, sizeof(pe));
    pe.dwSize = sizeof(pe);

    if (!Process32First(snap, &pe)) {
        CloseHandle(snap);
        printf("Nenhum processo encontrado.\n");
        ui_wait_enter();
        return;
    }

    do {
        printf("%6lu  %s\n", (unsigned long)pe.th32ProcessID, pe.szExeFile);
        count++;

        if (count % 25 == 0) {
            char line[8];
            printf("-- ENTER para continuar, 0 para voltar: ");
            if (!fgets(line, sizeof(line), stdin)) {
                break;
            }
            if (line[0] == '0') {
                break;
            }
        }
    } while (Process32Next(snap, &pe));

    CloseHandle(snap);
#else
    printf("Listagem de processos disponivel apenas em Windows.\n");
#endif

    ui_wait_enter();
}
