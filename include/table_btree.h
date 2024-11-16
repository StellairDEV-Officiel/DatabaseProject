#ifndef TABLE_BTREE_H
#define TABLE_BTREE_H
#include "table.h"

typedef struct TableNode {
    char name[50];
    Table* table;
    struct TableNode *left;
    struct TableNode *right;
} TableNode;

TableNode* insertTableNode(TableNode* root, char* name, Table* table);
TableNode* searchTableNode(TableNode* root, char* name);
TableNode* deleteTableNode(TableNode* root, char* name);
TableNode* findMinTableNode(TableNode* root);
void viewKeyTableNode(TableNode* root);
void freeTableTree(TableNode* root);

#endif //TABLE_BTREE_H
