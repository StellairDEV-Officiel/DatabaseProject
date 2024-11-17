#ifndef STORAGE_H
#define STORAGE_H
#include <stdio.h>
#include "table_btree.h"

void loadTableNode(TableNode** root, FILE* file);
void saveTableNode(TableNode* node, FILE* file);
void loadRecords(Table* table);
void saveAllTables(TableNode* root);
void loadAllTables(TableNode** root, int* tableCount);

#endif //STORAGE_H
