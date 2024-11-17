#include "../include/storage.h"
#include  "../include/table_btree.h"
#include  "../include/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveTableNode(TableNode* node, FILE* file) {
    if (node == NULL)
        return;
    saveTableNode(node->left, file);
    fprintf(file, "%s\n", node->name);
    fprintf(file, "%d\n", node->table->schema.columnCount);
    for (int j = 0; j < node->table->schema.columnCount; j++) {
        fprintf(file, "%s %d\n", node->table->schema.columns[j].name, node->table->schema.columns[j].type);
    }
    saveTableNode(node->right, file);
}

void saveAllTables(TableNode* root) {
    FILE* file = fopen("tables.dat", "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de base de donnée.\n");
        return;
    }

    int count = 0;
    typedef struct CountNode {
        TableNode* tableNode;
        struct CountNode* next;
    } CountNode;

    CountNode* countStack = NULL;
    TableNode* current = root;

    while (current != NULL || countStack != NULL) {
        while (current != NULL) {
            CountNode* newCountNode = (CountNode*)malloc(sizeof(CountNode));
            if (!newCountNode) {
                exit(EXIT_FAILURE);
            }
            newCountNode->tableNode = current;
            newCountNode->next = countStack;
            countStack = newCountNode;
            current = current->left;
        }

        if (countStack != NULL) {
            CountNode* temp = countStack;
            countStack = countStack->next;
            current = temp->tableNode;
            count++;
            free(temp);
            current = current->right;
        }
    }

    fprintf(file, "%d\n", count);

    saveTableNode(root, file);

    fclose(file);
    printf("Les tables ont été sauvegardées avec succès.\n");
}

void loadRecords(Table* table) {
    char dataFilename[60];
    snprintf(dataFilename, sizeof(dataFilename), "%s_data.dat", table->name);
    FILE* dataFile = fopen(dataFilename, "rb");
    if (dataFile == NULL) {
        printf("Aucun fichier de données trouvé pour la table '%s'.\n", table->name);
        return;
    }

    long offset = 0;
    while (1) {
        offset = ftell(dataFile);
        int columnCount;
        const size_t read = fread(&columnCount, sizeof(int), 1, dataFile);
        if (read != 1) {
            break; // Fin du fichier
        }

        Value* values = (Value*)malloc(columnCount * sizeof(Value));
        if (!values) {
            perror("Erreur d'allocation mémoire pour les valeurs");
            fclose(dataFile);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < columnCount; i++) {
            fread(&values[i].type, sizeof(DataType), 1, dataFile);
            if (values[i].type == TYPE_INT) {
                fread(&values[i].intValue, sizeof(int), 1, dataFile);
            } else if (values[i].type == TYPE_STRING) {
                fread(values[i].stringValue, sizeof(char), MAX_COLUMN_NAME_LENGTH, dataFile);
                values[i].stringValue[MAX_COLUMN_NAME_LENGTH - 1] = '\0'; // Assurer la terminaison
            }
        }

        // Obtenir la clé primaire
        int key;
        if (table->schema.columns[0].type == TYPE_INT) {
            key = values[0].intValue;
        } else {
            printf("La clé primaire n'est pas de type entier dans la table '%s'.\n", table->name);
            free(values);
            continue;
        }

        // Insérer la clé et l'offset dans l'arbre binaire des enregistrements
        table->root = insertRecordNode(table->root, key, offset);

        free(values);
    }

    fclose(dataFile);
}

void loadTableNode(TableNode** root, FILE* file) {
    char tableName[50];
    while (fscanf(file, "%s\n", tableName) == 1) {
        Table* newTable = (Table*)malloc(sizeof(Table));
        if (!newTable) {
            fclose(file);
            exit(EXIT_FAILURE);
        }
        strcpy(newTable->name, tableName);

        fscanf(file, "%d\n", &newTable->schema.columnCount);
        newTable->schema.columns = (Column*)malloc(newTable->schema.columnCount * sizeof(Column));
        if (!newTable->schema.columns) {
            free(newTable);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < newTable->schema.columnCount; j++) {
            fscanf(file, "%s %d\n", newTable->schema.columns[j].name, (int*)&newTable->schema.columns[j].type);
        }

        newTable->root = NULL;

        *root = insertTableNode(*root, tableName, newTable);

        loadRecords(newTable);
    }
}

void loadAllTables(TableNode** root, int* tableCount) {
    FILE* file = fopen("tables.dat", "r");
    if (file == NULL) {
        *root = NULL;
        *tableCount = 0;
        return;
    }

    fscanf(file, "%d\n", tableCount);
    loadTableNode(root, file);
    fclose(file);
}
