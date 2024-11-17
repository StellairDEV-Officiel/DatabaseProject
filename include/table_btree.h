#ifndef TABLE_BTREE_H
#define TABLE_BTREE_H

#define MAX_COLUMNS 10
#define MAX_COLUMNS_LENGTH 50
#define MAX_COLUMN_NAME_LENGTH 50
#define MAX_VALUE_LENGTH 50
#define MAX_TABLE_NAME_LENGTH 50
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

typedef struct TableNode {
    char name[MAX_TABLE_NAME_LENGTH];
    Table* table;
    struct TableNode *left;
    struct TableNode *right;
} TableNode;

typedef struct StackTableNode {
    TableNode* tableNode;
    struct StackTableNode* next;
} StackTableNode;

TableNode* insertTableNode(TableNode* root, const char* name, Table* table);
TableNode* searchTableNode(TableNode* root, const char* name);
TableNode* deleteTableNode(TableNode* root, const char* name);
TableNode* findMinTableNode(TableNode* root);
void viewKeyTableNode(TableNode* root);
void freeTableTree(TableNode* root);

#endif //TABLE_BTREE_H
