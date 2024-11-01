#include "../include/record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Record* insertRecord(Record* root, int id, int column_count, char columns[][MAX_VALUE_LENGTH]) {
    if (root == NULL) {
        Record* newRecord = (Record*)malloc(sizeof(Record));
        newRecord->id = id;

        for (int i = 0; i < column_count; i++) {
            strcpy(newRecord->columns[i].value, columns[i]);
        }
        newRecord->left = newRecord->right = NULL;
        return newRecord;
    }

    if (id < root->id) {
        root->left = insertRecord(root->left, id, column_count, columns);
    } else if (id > root->id) {
        root->right = insertRecord(root->right, id, column_count, columns);
    }
    return root;
}

Record* deleteRecord(Record* root, int id) {
    //TODO faire la fonction de supression des enregsitrement quand sa serra bon côté
    return root;
}

void selectRecords(Record* root, Column schema[], int column_count) {
    if (root != NULL) {
        selectRecords(root->left, schema, column_count);
        printf("ID: %d", root->id);
        for (int i = 0; i < column_count; i++) {
            printf(", %s: %s", schema[i].name, root->columns[i].value);
        }
        printf("\n");
        selectRecords(root->right, schema, column_count);
    }
}