#ifndef RECORD_H
#define RECORD_H

#include "table.h"

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

Record* insertRecord(Record* root, int id, int column_count, char columns[][MAX_VALUE_LENGTH]);
Record* deleteRecord(Record* root, int id);
void selectRecords(Record* root, Column schema[], int column_count);

#endif //RECORD_H
