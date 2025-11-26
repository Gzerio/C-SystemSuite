#ifndef DB_H
#define DB_H

#define DB_MAX_NAME 64
#define DB_MAX_VALUE 128

typedef struct {
    int id;
    char name[DB_MAX_NAME];
    char value[DB_MAX_VALUE];
    int active;
} DbRecord;

void db_main();

int db_storage_load(const char* filename, DbRecord** out_records, size_t* out_count);
int db_storage_save(const char* filename, const DbRecord* records, size_t count);

#endif
