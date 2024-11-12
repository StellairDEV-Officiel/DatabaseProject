#ifndef TABLE_H
#define TABLE_H

#define MAX_COLUMNS 10
#define MAX_COLUMNS_LENGTH 10
#define MAX_COLUMN_NAME_LENGTH 50
#define MAX_VALUE_LENGTH 256

typedef struct Column {
    char name[MAX_COLUMN_NAME_LENGTH];
    char value[MAX_VALUE_LENGTH];
} Column;

typedef struct Record {
    int id;
    Column columns[MAX_COLUMNS_LENGTH];
    struct Record* left;
    struct Record* right;
} Record;

typedef struct Table {
    char name[MAX_COLUMN_NAME_LENGTH];
    Column schema[MAX_COLUMNS_LENGTH];
    int column_count;
    Record* records;
    struct Table* next;
} Table;

// TODO intégrer les tables dans database
// TODO passer les Tables de liste chainé à arbre binaire comme pour les records

Table* createTable(Table** database, const char* name, int column_count, char columns[][MAX_COLUMN_NAME_LENGTH]);
Table* findTable(Table* database, const char* name);


#endif //TABLE_H
