#include <stdio.h>
#include <string.h>
#include "http.h"
#include "ui.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

int http_run_server(int port) {
    WSADATA wsa;
    SOCKET server;
    SOCKET client;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    char buffer[2048];
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erro WSAStartup.\n");
        return 1;
    }

    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        printf("Erro ao criar socket.\n");
        WSACleanup();
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((u_short)port);

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Erro no bind.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, 5) == SOCKET_ERROR) {
        printf("Erro no listen.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    printf("Servidor HTTP na porta %d.\n", port);
    printf("Acesse: http://localhost:%d/\n", port);
    printf("CTRL+C para parar.\n");

    while (1) {
        client = accept(server, (struct sockaddr*)&addr, &addrlen);
        if (client == INVALID_SOCKET) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        result = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (result > 0) {
            char response[4096];
            http_build_response(buffer, response, sizeof(response));
            send(client, response, (int)strlen(response), 0);
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}

#else

int http_run_server(int port) {
    printf("Servidor HTTP nao suportado neste sistema.\n");
    return 1;
}

#endif

void http_main() {
    int opcao = -1;

    while (opcao != 0) {
        ui_clear();
        ui_header("SERVIDOR HTTP");

        printf("[1] Iniciar servidor na porta 8080\n");
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
            ui_header("SERVIDOR HTTP");
            printf("Servidor rodando na porta 8080.\n");
            printf("Use CTRL+C para parar.\n\n");
            http_run_server(8080);
            ui_wait_enter();
        }
    }
}

