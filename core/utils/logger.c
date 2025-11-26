#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include "core/utils/logger.h"

static FILE* logger_out = NULL;

static void logger_write(const char* level, const char* fmt, va_list args) {
    time_t t = time(NULL);
    struct tm tmv;
#if defined(_WIN32)
    localtime_s(&tmv, &t);
#else
    localtime_r(&t, &tmv);
#endif

    if (!logger_out) {
        logger_out = stderr;
    }

    fprintf(logger_out, "[%02d:%02d:%02d] %s ",
            tmv.tm_hour, tmv.tm_min, tmv.tm_sec, level);
    vfprintf(logger_out, fmt, args);
    fputc('\n', logger_out);
    fflush(logger_out);
}

void logger_init(const char* path) {
    if (logger_out && logger_out != stderr) {
        fclose(logger_out);
        logger_out = NULL;
    }

    if (path && path[0]) {
        FILE* f = fopen(path, "a");
        if (f) {
            logger_out = f;
            return;
        }
    }

    logger_out = stderr;
}

void logger_close() {
    if (logger_out && logger_out != stderr) {
        fclose(logger_out);
    }
    logger_out = NULL;
}

void logger_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write("[INFO]", fmt, args);
    va_end(args);
}

void logger_warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write("[WARN]", fmt, args);
    va_end(args);
}

void logger_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_write("[ERRO]", fmt, args);
    va_end(args);
}
