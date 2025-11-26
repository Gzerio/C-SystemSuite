#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "ui.h"
#include "core/utils/logger.h"
#include "core/memory/allocator.h"

static DbRecord* g_records = NULL;
static size_t g_count = 0;
static int g_next_id = 1;
static char g_table[64] = "default";
static char g_file[128] = "csystemsuite_db_default.bin";

static void db_update_file() {
    snprintf(g_file, sizeof(g_file), "csystemsuite_db_%s.bin", g_table);
}

static void db_load() {
    DbRecord* records = NULL;
    size_t count = 0;
    int ok;
    size_t i;
    int max_id = 0;

    db_update_file();
    ok = db_storage_load(g_file, &records, &count);

    if (!ok) {
        logger_error("db_storage_load falhou para %s", g_file);
        return;
    }

    if (g_records) {
        mem_free(g_records);
        g_records = NULL;
        g_count = 0;
    }

    g_records = records;
    g_count = count;

    for (i = 0; i < g_count; i++) {
        if (g_records[i].id > max_id) {
            max_id = g_records[i].id;
        }
    }

    g_next_id = max_id + 1;
    if (g_next_id < 1) {
        g_next_id = 1;
    }

    logger_info("DB carregado: tabela=%s registros=%zu", g_table, g_count);
}

static void db_save() {
    db_update_file();
    if (!db_storage_save(g_file, g_records, g_count)) {
        logger_error("Falha ao salvar DB em %s", g_file);
        printf("Falha ao salvar banco.\n");
    } else {
        logger_info("DB salvo: tabela=%s registros=%zu", g_table, g_count);
    }
}

static void db_list() {
    size_t i;
    int any = 0;

    ui_clear();
    {
        char title[128];
        snprintf(title, sizeof(title), "DB - LISTAR REGISTROS [tabela: %s]", g_table);
        ui_header(title);
    }

    for (i = 0; i < g_count; i++) {
        if (g_records[i].active) {
            printf("ID: %d\n", g_records[i].id);
            printf("Nome: %s\n", g_records[i].name);
            printf("Valor: %s\n", g_records[i].value);
            printf("-----------------------------\n");
            any = 1;
        }
    }

    if (!any) {
        printf("Nenhum registro ativo.\n");
    }

    ui_wait_enter();
}

static void db_add() {
    char name[DB_MAX_NAME];
    char value[DB_MAX_VALUE];
    DbRecord* new_array;
    DbRecord rec;

    ui_clear();
    {
        char title[128];
        snprintf(title, sizeof(title), "DB - NOVO REGISTRO [tabela: %s]", g_table);
        ui_header(title);
    }

    printf("Nome (0 para voltar): ");
    if (!fgets(name, sizeof(name), stdin)) {
        return;
    }
    name[strcspn(name, "\n")] = 0;
    if (ui_line_is_back(name)) {
        return;
    }
    if (name[0] == 0) {
        printf("Nome vazio.\n");
        ui_wait_enter();
        return;
    }

    printf("Valor (0 para voltar): ");
    if (!fgets(value, sizeof(value), stdin)) {
        return;
    }
    value[strcspn(value, "\n")] = 0;
    if (ui_line_is_back(value)) {
        return;
    }
    if (value[0] == 0) {
        printf("Valor vazio.\n");
        ui_wait_enter();
        return;
    }

    rec.id = g_next_id++;
    strncpy(rec.name, name, DB_MAX_NAME - 1);
    rec.name[DB_MAX_NAME - 1] = 0;
    strncpy(rec.value, value, DB_MAX_VALUE - 1);
    rec.value[DB_MAX_VALUE - 1] = 0;
    rec.active = 1;

    new_array = (DbRecord*)mem_realloc(g_records, (g_count + 1) * sizeof(DbRecord));
    g_records = new_array;
    g_records[g_count] = rec;
    g_count++;

    db_save();

    logger_info("DB novo registro: tabela=%s id=%d", g_table, rec.id);

    printf("Registro criado com ID %d.\n", rec.id);
    ui_wait_enter();
}

static DbRecord* db_find_by_id(int id) {
    size_t i;
    for (i = 0; i < g_count; i++) {
        if (g_records[i].active && g_records[i].id == id) {
            return &g_records[i];
        }
    }
    return NULL;
}

static void db_search() {
    char line[32];
    int id;
    DbRecord* rec;

    ui_clear();
    {
        char title[128];
        snprintf(title, sizeof(title), "DB - BUSCAR POR ID [tabela: %s]", g_table);
        ui_header(title);
    }

    printf("ID (0 para voltar): ");
    if (!fgets(line, sizeof(line), stdin)) {
        return;
    }
    line[strcspn(line, "\n")] = 0;
    if (ui_line_is_back(line)) {
        return;
    }

    id = atoi(line);
    if (id == 0) {
        return;
    }

    rec = db_find_by_id(id);
    if (!rec) {
        printf("Registro nao encontrado.\n");
        logger_warn("DB busca falhou: tabela=%s id=%d", g_table, id);
    } else {
        printf("ID: %d\n", rec->id);
        printf("Nome: %s\n", rec->name);
        printf("Valor: %s\n", rec->value);
        logger_info("DB busca ok: tabela=%s id=%d", g_table, id);
    }

    ui_wait_enter();
}

static void db_delete() {
    char line[32];
    int id;
    DbRecord* rec;

    ui_clear();
    {
        char title[128];
        snprintf(title, sizeof(title), "DB - REMOVER REGISTRO [tabela: %s]", g_table);
        ui_header(title);
    }

    printf("ID para remover (0 para voltar): ");
    if (!fgets(line, sizeof(line), stdin)) {
        return;
    }
    line[strcspn(line, "\n")] = 0;
    if (ui_line_is_back(line)) {
        return;
    }

    id = atoi(line);
    if (id == 0) {
        return;
    }

    rec = db_find_by_id(id);
    if (!rec) {
        printf("Registro nao encontrado.\n");
        logger_warn("DB remover falhou: tabela=%s id=%d", g_table, id);
    } else {
        rec->active = 0;
        db_save();
        logger_info("DB registro removido: tabela=%s id=%d", g_table, id);
        printf("Registro %d marcado como removido.\n", id);
    }

    ui_wait_enter();
}

static void db_edit() {
    char line[32];
    char buf[DB_MAX_VALUE];
    int id;
    DbRecord* rec;

    ui_clear();
    {
        char title[128];
        snprintf(title, sizeof(title), "DB - EDITAR REGISTRO [tabela: %s]", g_table);
        ui_header(title);
    }

    printf("ID para editar (0 para voltar): ");
    if (!fgets(line, sizeof(line), stdin)) {
        return;
    }
    line[strcspn(line, "\n")] = 0;
    if (ui_line_is_back(line)) {
        return;
    }

    id = atoi(line);
    if (id == 0) {
        return;
    }

    rec = db_find_by_id(id);
    if (!rec) {
        printf("Registro nao encontrado.\n");
        logger_warn("DB editar falhou: tabela=%s id=%d", g_table, id);
        ui_wait_enter();
        return;
    }

    printf("ID: %d\n", rec->id);
    printf("Nome atual: %s\n", rec->name);
    printf("Valor atual: %s\n", rec->value);

    printf("Novo nome (ENTER para manter, 0 para voltar): ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        return;
    }
    buf[strcspn(buf, "\n")] = 0;
    if (ui_line_is_back(buf)) {
        return;
    }
    if (buf[0] != 0) {
        strncpy(rec->name, buf, DB_MAX_NAME - 1);
        rec->name[DB_MAX_NAME - 1] = 0;
    }

    printf("Novo valor (ENTER para manter, 0 para voltar): ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        return;
    }
    buf[strcspn(buf, "\n")] = 0;
    if (ui_line_is_back(buf)) {
        return;
    }
    if (buf[0] != 0) {
        strncpy(rec->value, buf, DB_MAX_VALUE - 1);
        rec->value[DB_MAX_VALUE - 1] = 0;
    }

    db_save();
    logger_info("DB registro editado: tabela=%s id=%d", g_table, id);

    printf("Registro %d atualizado.\n", rec->id);
    ui_wait_enter();
}

static void db_change_table() {
    char name[64];

    ui_clear();
    ui_header("DB - TROCAR TABELA");

    printf("Nome da tabela (0 para voltar): ");
    if (!fgets(name, sizeof(name), stdin)) {
        return;
    }
    name[strcspn(name, "\n")] = 0;
    if (ui_line_is_back(name)) {
        return;
    }
    if (name[0] == 0) {
        printf("Nome vazio.\n");
        ui_wait_enter();
        return;
    }

    strncpy(g_table, name, sizeof(g_table) - 1);
    g_table[sizeof(g_table) - 1] = 0;

    db_load();

    printf("Tabela atual: %s\n", g_table);
    ui_wait_enter();
}

void db_main() {
    char line[16];
    int opcao;

    db_load();

    while (1) {
        ui_clear();
        {
            char title[128];
            snprintf(title, sizeof(title), "C-SystemSuite DB [tabela: %s]", g_table);
            ui_header(title);
        }

        printf("[1] Listar registros\n");
        printf("[2] Novo registro\n");
        printf("[3] Buscar por ID\n");
        printf("[4] Remover por ID\n");
        printf("[5] Editar registro\n");
        printf("[6] Trocar tabela\n");
        printf("[0] Voltar\n");
        printf("Opcao: ");

        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        if (line[0] == '0') {
            break;
        }

        opcao = atoi(line);

        if (opcao == 1) {
            db_list();
        } else if (opcao == 2) {
            db_add();
        } else if (opcao == 3) {
            db_search();
        } else if (opcao == 4) {
            db_delete();
        } else if (opcao == 5) {
            db_edit();
        } else if (opcao == 6) {
            db_change_table();
        }
    }

    if (g_records) {
        mem_free(g_records);
        g_records = NULL;
        g_count = 0;
    }

    logger_info("DB modulo encerrado, tabela=%s", g_table);
}
