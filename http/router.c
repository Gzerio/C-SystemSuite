#include <stdio.h>
#include <string.h>
#include <time.h>
#include "http.h"

void http_build_response(const char* request, char* buffer, int buffer_size) {
    char method[8];
    char path[256];
    char body[4096];
    char datetime[64];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    static time_t server_start = 0;
    static unsigned long total_requests = 0;
    long uptime;

    if (server_start == 0) {
        server_start = now;
    }

    total_requests++;

    if (t != NULL) {
        strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", t);
    } else {
        strcpy(datetime, "desconhecido");
    }

    method[0] = 0;
    path[0] = 0;
    sscanf(request, "%7s %255s", method, path);

    uptime = (long)(now - server_start);

    if (strcmp(path, "/status") == 0) {
        snprintf(body, sizeof(body),
                 "{"
                 "\"uptime_seconds\": %ld,"
                 "\"total_requests\": %lu,"
                 "\"server_time\": \"%s\""
                 "}",
                 uptime,
                 total_requests,
                 datetime);

        snprintf(buffer, buffer_size,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json; charset=utf-8\r\n"
                 "Content-Length: %d\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "%s",
                 (int)strlen(body),
                 body);
    } else {
        snprintf(body, sizeof(body),
                 "<html><head><title>C-SystemSuite</title>"
                 "<style>"
                 "body{font-family:sans-serif;background:#020617;color:#e5e7eb;display:flex;flex-direction:column;justify-content:center;align-items:center;min-height:100vh;margin:0;}"
                 ".card{padding:24px 32px;border-radius:16px;background:rgba(15,23,42,0.95);box-shadow:0 24px 60px rgba(0,0,0,0.6);border:1px solid rgba(148,163,184,0.5);min-width:320px;max-width:480px;}"
                 ".title{font-size:26px;margin-bottom:4px;}"
                 ".subtitle{font-size:14px;color:#9ca3af;margin-bottom:16px;}"
                 ".meta{font-size:14px;color:#e5e7eb;margin-bottom:4px;}"
                 ".label{color:#9ca3af;}"
                 ".links{margin-top:16px;font-size:14px;}"
                 "a{color:#38bdf8;text-decoration:none;}"
                 "a:hover{text-decoration:underline;}"
                 ".btn{display:inline-block;margin-top:16px;padding:10px 18px;border-radius:999px;background:#0f172a;border:1px solid #38bdf8;color:#e5e7eb;font-size:14px;font-weight:500;text-decoration:none;box-shadow:0 12px 30px rgba(56,189,248,0.3);}"
                 ".btn:hover{background:#020617;}"
                 ".footer-time{margin-top:18px;font-size:13px;color:#9ca3af;text-align:center;}"
                 "</style></head>"
                 "<body>"
                 "<div class=\"card\">"
                 "<div class=\"title\">C-SystemSuite HTTP</div>"
                 "<div class=\"subtitle\">Mini servidor embutido em C</div>"
                 "<div class=\"meta\"><span class=\"label\">Rota atual:</span> %s</div>"
                 "<div class=\"meta\"><span class=\"label\">Uptime:</span> %ld s</div>"
                 "<div class=\"meta\"><span class=\"label\">Total de requests:</span> %lu</div>"
                 "<div class=\"meta\"><span class=\"label\">Horario do servidor:</span> %s</div>"
                 "<div class=\"links\">"
                 "<div><a href=\"/status\">/status</a> &mdash; JSON com status</div>"
                 "</div>"
                 "<a class=\"btn\" href=\"https://github.com/Gzerio/C-SystemSuite\" target=\"_blank\">Click aqui</a>"
                 "</div>"
                 "<div class=\"footer-time\">Servidor ativo em %s</div>"
                 "</body></html>",
                 path[0] ? path : "/",
                 uptime,
                 total_requests,
                 datetime,
                 datetime);

        snprintf(buffer, buffer_size,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=utf-8\r\n"
                 "Content-Length: %d\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "%s",
                 (int)strlen(body),
                 body);
    }
}
