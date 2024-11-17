#include "../include/database.h"
#include <stdlib.h>
#include <string.h>

void createTable(TableNode** root, char* tableName, InputBuffer* input_buffer) {
    if (searchTableNode(*root, tableName) != NULL) {
        printf("La table '%s' existe déjà.\n", tableName);
        return;
    }

    Table* newTable = (Table*)malloc(sizeof(Table));
    strncpy(newTable->name, tableName, MAX_TABLE_NAME_LENGTH);

    newTable->schema.columns = (Column*)malloc(MAX_COLUMNS * sizeof(Column));

    int columnCount = 0;
    while (1) {
        printf("Entrez le nom de la colonne numéro %d (ou appuyez sur Entrée pour terminer) >", columnCount + 1);
        read_input(input_buffer);

        if (strncmp(input_buffer->buffer, "", 1) == 0)
        {
            printf("Fin de la création de la table.\n");
            break;
        }

        if (columnCount >= MAX_COLUMNS) {
            printf("Nombre maximal de colonnes atteint.\n");
            break;
        }

        strncpy(newTable->schema.columns[columnCount].name, input_buffer->buffer, MAX_COLUMN_NAME_LENGTH);

        printf("Type de la colonne '%s' (1=int, 2=string) >", newTable->schema.columns[columnCount].name);
        read_input(input_buffer);

        if (strncmp(input_buffer->buffer, "1", 1) == 0) {
            newTable->schema.columns[columnCount].type = TYPE_INT;
        } else {
            newTable->schema.columns[columnCount].type = TYPE_STRING;
        }

        columnCount++;
    }

    newTable->schema.columnCount = columnCount;
    newTable->root = NULL;

    *root = insertTableNode(*root, tableName, newTable);

    char dataFileName[60];
    snprintf(dataFileName, sizeof(dataFileName), "%s_data.dat", tableName);
    FILE* dataFile = fopen(dataFileName, "wb");
    if (dataFile != NULL) {
        fclose(dataFile);
        printf("Table '%s' créée avec succès.\n", tableName);
    } else {
        printf("Erreur lors de la création du fichier pour la table '%s'.\n", tableName);
    }
}

void insertRecord(TableNode* root, char* tableName, InputBuffer* input_buffer) {
    if (root == NULL) {
        printf("Aucune table existante.\n");
        return;
    }

    const TableNode* tableNode = searchTableNode(root, tableName);
    if (tableNode == NULL) {
        printf("La table '%s' n'existe pas.\n", tableName);
        return;
    }

    Table* table = tableNode->table;

    Record record;
    const int columnCount = table->schema.columnCount;
    record.values = (Value*)malloc(columnCount * sizeof(Value));

    for (int i = 0; i < columnCount; i++) {
        Column* col = &table->schema.columns[i];
        printf("Entrez la valeur pour '%s' (%s): ", col->name, col->type == TYPE_INT ? "int" : "string");
        if (col->type == TYPE_INT) {
            record.values[i].type = TYPE_INT;
            scanf("%d", &record.values[i].intValue);
            getchar(); //TODO remplacer scanf car pas adapté, mais j'ai un soucis pour gérer les int donc pour le moment je reste sur scanf pour les int
        } else if (col->type == TYPE_STRING) {
            record.values[i].type = TYPE_STRING;
            scanf("%s", record.values[i].stringValue);
            getchar();
        }
    }

    int key = 0;
    if (table->schema.columns[0].type == TYPE_INT) {
        key = record.values[0].intValue;
    } else {
        printf("La clé primaire doit être un entier.\n");
        free(record.values);
        return;
    }

    if (searchRecordNode(table->root, key) != NULL) {
        printf("Erreur : Une entrée avec la clé '%d' existe déjà dans la table '%s'.\n", key, tableName);
        free(record.values);
        return;
    }


    char dataFilename[60];
    sprintf(dataFilename, "%s_data.dat", tableName);
    FILE* dataFile = fopen(dataFilename, "ab"); //binarymode
    if (dataFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de données.\n");
        free(record.values);
        return;
    }

    fseek(dataFile, 0, SEEK_END);
    const long offset = ftell(dataFile);

    fwrite(&table->schema.columnCount, sizeof(int), 1, dataFile); // Nombre de colonnes
    for (int i = 0; i < columnCount; i++) {
        fwrite(&record.values[i].type, sizeof(DataType), 1, dataFile); // Type de la valeur
        if (record.values[i].type == TYPE_INT) {
            fwrite(&record.values[i].intValue, sizeof(int), 1, dataFile); // Valeur entière
        } else if (record.values[i].type == TYPE_STRING) {
            fwrite(record.values[i].stringValue, sizeof(char), 50, dataFile); // Valeur chaîne
        }
    }
    fclose(dataFile);

    table->root = insertRecordNode(table->root, key, offset);
    printf("Enregistrement inséré dans la table '%s' avec la clé '%d'.\n", tableName, key);

    free(record.values);
}

void selectRecord(TableNode* root, char* tableName, const int key) {
    if (root == NULL) {
        printf("La table '%s' n'existe pas.\n", tableName);
        return;
    }

    const TableNode* tableNode = searchTableNode(root, tableName);
    if (tableNode == NULL) {
        printf("La table '%s' n'existe pas.\n", tableName);
        return;
    }

    const Table* table = tableNode->table;

    const Node* node = searchRecordNode(table->root, key);
    if (node == NULL) {
        printf("Clé '%d' non trouvée dans la table '%s'.\n", key, tableName);
        return;
    }

    char dataFilename[60];
    sprintf(dataFilename, "%s_data.dat", tableName);
    FILE* dataFile = fopen(dataFilename, "rb");
    if (dataFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de données.\n");
        return;
    }
    fseek(dataFile, node->offset, SEEK_SET);

    int columnCount;
    if (fread(&columnCount, sizeof(int), 1, dataFile) != 1) {
        printf("Erreur de lecture du nombre de colonnes.\n");
        fclose(dataFile);
        return;
    }

    Record record;
    record.values = (Value*)malloc(columnCount * sizeof(Value));

    for (int i = 0; i < columnCount; i++) {
        if (fread(&record.values[i].type, sizeof(DataType), 1, dataFile) != 1) {
            printf("Erreur de lecture du type de donnée.\n");
            free(record.values);
            fclose(dataFile);
            return;
        }
        if (record.values[i].type == TYPE_INT) {
            if (fread(&record.values[i].intValue, sizeof(int), 1, dataFile) != 1) {
                printf("Erreur de lecture de la valeur entière.\n");
                free(record.values);
                fclose(dataFile);
                return;
            }
        } else if (record.values[i].type == TYPE_STRING) {
            if (fread(record.values[i].stringValue, sizeof(char), 50, dataFile) != 50) {
                printf("Erreur de lecture de la valeur chaîne.\n");
                free(record.values);
                fclose(dataFile);
                return;
            }
            record.values[i].stringValue[49] = '\0';
        }
    }
    fclose(dataFile);

    printf("Enregistrement trouvé dans la table '%s' avec la clé '%d':\n", tableName, key);
    for (int i = 0; i < columnCount; i++) {
        printf("%s: ", table->schema.columns[i].name);
        if (record.values[i].type == TYPE_INT) {
            printf("%d\n", record.values[i].intValue);
        } else if (record.values[i].type == TYPE_STRING) {
            printf("%s\n", record.values[i].stringValue);
        }
    }

    free(record.values);
}

void deleteRecord(TableNode** root, char* tableName, const int key) {
    if (*root == NULL) {
        printf("Aucune table existante.\n");
        return;
    }

    const TableNode* tableNode = searchTableNode(*root, tableName);
    if (tableNode == NULL) {
        printf("La table '%s' n'existe pas.\n", tableName);
        return;
    }

    Table* table = tableNode->table;

    const Node* node = searchRecordNode(table->root, key);
    if (node == NULL) {
        printf("Clé '%d' non trouvée dans la table '%s'.\n", key, tableName);
        return;
    }

    char dataFileName[60];
    sprintf(dataFileName, "%s_data.dat", tableName);
    FILE* dataFile = fopen(dataFileName, "rb+");
    if (dataFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de données.\n");
        return;
    }
    fseek(dataFile, node->offset, SEEK_SET);

    int columnCount;
    if (fread(&columnCount, sizeof(int), 1, dataFile) != 1) {
        printf("Erreur de lecture du nombre de colonnes.\n");
        fclose(dataFile);
        return;
    }

    DataType type;
    if (fread(&type, sizeof(DataType), 1, dataFile) != 1) {
        printf("Erreur de lecture du type de donnée.\n");
        fclose(dataFile);
        return;
    }

    if (type != TYPE_INT) {
        printf("La clé primaire n'est pas de type entier. Suppression impossible.\n");
        fclose(dataFile);
        return;
    }

    int dummyValue = -1; // Valeur pour marquer comme supprimé
    if (fwrite(&dummyValue, sizeof(int), 1, dataFile) != 1) {
        printf("Erreur de marquage de l'enregistrement comme supprimé.\n");
        fclose(dataFile);
        return;
    }

    fclose(dataFile);

    table->root = deleteRecordNode(table->root, key);
    printf("Enregistrement avec la clé '%d' supprimé de la table '%s'.\n", key, tableName);
}