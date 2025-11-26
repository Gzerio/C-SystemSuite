#include <stdlib.h>
#include "core/utils/logger.h"
#include "core/memory/allocator.h"

void* mem_alloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        logger_error("mem_alloc falhou para %zu bytes", size);
        exit(1);
    }
    return ptr;
}

void* mem_realloc(void* ptr, size_t size) {
    void* out = realloc(ptr, size);
    if (!out) {
        logger_error("mem_realloc falhou para %zu bytes", size);
        exit(1);
    }
    return out;
}

void mem_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}
