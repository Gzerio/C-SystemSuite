#ifndef LOGGER_H
#define LOGGER_H

void logger_init(const char* path);
void logger_close();

void logger_info(const char* fmt, ...);
void logger_warn(const char* fmt, ...);
void logger_error(const char* fmt, ...);

#endif
