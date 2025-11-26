#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void monitor_system_info();
void monitor_process_overview();

static void monitor_system_info() {
    ui_clear();
    ui_header("MONITOR - SISTEMA");

#ifdef _WIN32
    SYSTEM_INFO si;
    MEMORYSTATUSEX mem;
    DWORDLONG uptime_ms;
    DWORDLONG total_mb;
    DWORDLONG avail_mb;

    GetNativeSystemInfo(&si);

    mem.dwLength = sizeof(mem);
    if (GlobalMemoryStatusEx(&mem)) {
        total_mb = (DWORDLONG)(mem.ullTotalPhys / (1024 * 1024));
        avail_mb = (DWORDLONG)(mem.ullAvailPhys / (1024 * 1024));
    } else {
        total_mb = 0;
        avail_mb = 0;
    }

    uptime_ms = GetTickCount64();
    unsigned long long total_sec = (unsigned long long)(uptime_ms / 1000);
    unsigned long long days = total_sec / 86400;
    unsigned long long hours = (total_sec % 86400) / 3600;
    unsigned long long minutes = (total_sec % 3600) / 60;
    unsigned long long seconds = total_sec % 60;

    printf("Sistema operacional: Windows\n");
    printf("Processadores lógicos: %lu\n", si.dwNumberOfProcessors);
    printf("Memoria fisica total: %llu MB\n", (unsigned long long)total_mb);
    printf("Memoria fisica livre: %llu MB\n", (unsigned long long)avail_mb);
    printf("Uptime: %llud %lluh %llum %llus\n", days, hours, minutes, seconds);
#else
    printf("Monitor de sistema detalhado disponivel apenas em Windows.\n");
#endif

    ui_wait_enter();
}

void monitor_main() {
    char line[16];
    int opcao;

    while (1) {
        ui_clear();
        ui_header("C-SystemSuite MONITOR");

        printf("[1] Informacoes do sistema\n");
        printf("[2] Processos\n");
        printf("[0] Voltar\n");
        printf("Opcao: ");

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\n")] = 0;
        if (ui_line_is_back(line)) {
            break;
        }

        opcao = atoi(line);

        if (opcao == 1) {
            monitor_system_info();
        } else if (opcao == 2) {
            monitor_process_overview();
        }
    }
}
