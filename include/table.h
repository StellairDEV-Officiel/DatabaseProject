#ifndef TABLE_H
#define TABLE_H

#define MAX_TABLE_NAME_LENGTH 50
#define MAX_COLUMN_NAME_LENGTH 50
#include "records_btree.h"

typedef enum DataType {
    TYPE_INT,
    TYPE_STRING
} DataType;

typedef struct Column {
    char name[MAX_COLUMN_NAME_LENGTH];
    DataType type;
} Column;

typedef struct Schema {
    int columnCount;
    Column* columns;
} Schema;

typedef struct Table {
    char name[MAX_TABLE_NAME_LENGTH];
    Schema schema;
    Node* root;
} Table;

Table* createTable(Table** database, const char* name, int column_count, char columns[][MAX_COLUMN_NAME_LENGTH]);
Table* findTable(Table* database, const char* name);


#endif //TABLE_H
