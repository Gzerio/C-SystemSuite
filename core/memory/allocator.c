#include <stdlib.h>
#include "allocator.h"

void* mem_alloc(int size) {
    return malloc(size);
}

void mem_free(void* ptr) {
    free(ptr);
}
