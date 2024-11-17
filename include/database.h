#ifndef DATABASE_H
#define DATABASE_H

#include "command.h"
#include "table_btree.h"

typedef struct Value {
    DataType type;
    union {
        int intValue;
        char stringValue[50];
    }; // ou CHAR ou INT
} Value;

typedef struct Record {
    Value* values;
} Record;

void createTable(TableNode** root, char* tableName, InputBuffer* input_buffer);
void insertRecord(TableNode* root, char* tableName);
void selectRecord(TableNode* root, char* tableName, int key);
void deleteRecord(TableNode** root, char* tableName, int key);

#endif //DATABASE_H
