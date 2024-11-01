#include "../include/table.h"
#include <stdlib.h>
#include <string.h>

Table* createTable(Table** database, const char* name, int column_count, char columns[][MAX_COLUMN_NAME_LENGTH]) {
    Table* newTable = (Table*)malloc(sizeof(Table));
    strcpy(newTable->name, name);
    newTable->column_count = column_count;
    for (int i = 0; i < column_count; i++) {
        strcpy(newTable->schema[i].name, columns[i]);
    }
    newTable->records = NULL;
    newTable->next = *database;
    *database = newTable;
    return newTable;
}

Table* findTable(Table* database, const char* name) {
    while (database != NULL) {
        if (strcmp(database->name, name) == 0) {
            return database;
        }
        database = database->next;
    }
    return NULL;
}