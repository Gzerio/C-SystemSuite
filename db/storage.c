#include <stdio.h>
#include <stdlib.h>
#include "db.h"

int db_storage_load(const char* filename, DbRecord** out_records, size_t* out_count) {
    FILE* f = fopen(filename, "rb");
    long size;
    size_t count;
    DbRecord* records;

    *out_records = NULL;
    *out_count = 0;

    if (!f) {
        return 1;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }

    size = ftell(f);
    if (size < 0) {
        fclose(f);
        return 0;
    }

    if (size == 0) {
        fclose(f);
        return 1;
    }

    if (size % (long)sizeof(DbRecord) != 0) {
        fclose(f);
        return 0;
    }

    count = (size_t)(size / (long)sizeof(DbRecord));
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return 0;
    }

    records = (DbRecord*)malloc(count * sizeof(DbRecord));
    if (!records) {
        fclose(f);
        return 0;
    }

    if (fread(records, sizeof(DbRecord), count, f) != count) {
        free(records);
        fclose(f);
        return 0;
    }

    fclose(f);

    *out_records = records;
    *out_count = count;
    return 1;
}

int db_storage_save(const char* filename, const DbRecord* records, size_t count) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }

    if (count > 0) {
        if (fwrite(records, sizeof(DbRecord), count, f) != count) {
            fclose(f);
            return 0;
        }
    }

    fclose(f);
    return 1;
}
