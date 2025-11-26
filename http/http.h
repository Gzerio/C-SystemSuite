#ifndef HTTP_H
#define HTTP_H

int http_run_server(int port);
void http_main();
void http_build_response(const char* request, char* buffer, int buffer_size);

#endif
