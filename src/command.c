// https://github.com/Mbahal/class_db/blob/main/src/repl.c
#include "../include/command.h"
#include "../include/record.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer *) malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("DatabaseProject> "); }

void read_input(InputBuffer *input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // Ignore trailing newline
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  }
  if (strcmp(input_buffer->buffer, ".help") == 0) {
    printf("+-------------------+---------------------------------------+\n");
    printf("| Commande          | Description                           |\n");
    printf("+-------------------+---------------------------------------+\n");
    printf("| create            | Créer une table                       |\n");
    printf("| insert            | Insérer un enregistrement             |\n");
    printf("| select            | Afficher tous les enregistrements     |\n");
    printf("| delete            | Supprimer un enregistrement par ID    |\n");
    printf("| .exit             | Quitter le programme                  |\n");
    printf("+-------------------+---------------------------------------+\n\n");
    return META_COMMAND_SUCCESS;
  }

  return META_COMMAND_UNRECOGNIZED_COMMAND;
}

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "create") == 0) {
    statement->type = STATEMENT_CREATE;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "delete") == 0) {
    statement->type = STATEMENT_DELETE;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

int getMaxId(Record *root)
{
  if (root == NULL)
  {
    return 0;
  }

  Record *current = root;
  while (current->right != NULL) // enregistrement le plus à droite à l'id le plus élevé
  {
    current = current->right;
  }
  return current->id;
}

void execute_statement(Statement *statement, Table **database, InputBuffer *input_buffer) {
  char tableName[MAX_COLUMN_NAME_LENGTH];
  Table *table;

  printf("Entrez le nom de la table>");
  read_input(input_buffer);
  strncpy(tableName, input_buffer->buffer, MAX_COLUMN_NAME_LENGTH);

  switch (statement->type) {
    case (STATEMENT_CREATE):
      char columns[MAX_COLUMNS][MAX_COLUMN_NAME_LENGTH];
      int columnCount = 0;

      while (1)
      {
        printf("Entrez le nom de la colonne numéro %d (ou appuyez sur Entrée pour terminer)>", columnCount + 1);
        read_input(input_buffer);
        strncpy(columns[columnCount], input_buffer->buffer, MAX_COLUMN_NAME_LENGTH);

        if (strncmp(columns[columnCount], "", 1) == 0)
        {
          printf("Fin de la création de la table.\n");
          break;
        }
        columnCount++;
        if (columnCount >= MAX_COLUMNS)
        {
          printf("Nombre maximal de colonnes atteint.\n");
          break;
        }
      }

      createTable(database, tableName, columnCount, columns);
      printf("Table '%s' créée avec %d colonnes.\n", tableName, columnCount);
      break;
    case (STATEMENT_INSERT):
      char values[MAX_COLUMNS][MAX_VALUE_LENGTH];

      table = findTable(*database, tableName);

      if (table != NULL)
      {
        int id = getMaxId(table->records) + 1;
        int valueCount = 0;

        printf("Entrez les valeurs pour chaque colonne (appuyez sur Entrée sans rien écrire pour terminer) :\n");
        for (int i = 0; i < table->column_count; i++)
        {
          printf("%s: ", table->schema[i].name);
          read_input(input_buffer);
          strncpy(values[i], input_buffer->buffer, MAX_VALUE_LENGTH);

          if (strncmp(values[i], "", 1) == 0)
          {
            printf("Fin de l'insertion des données de la table.\n");
            break;
          }
          valueCount++;
        }

        table->records = insertRecord(table->records, id, valueCount, values);
        printf("Enregistrement inséré avec l'ID %d.\n", id);
      }
      else
      {
        printf("Erreur : Table '%s' non trouvée.\n", tableName);
      }
      break;
    case (STATEMENT_SELECT):
      table = findTable(*database, tableName);

      if (table != NULL)
      {
        printf("id   |");
        for (int i = 0; i < table->column_count; i++)
        {
          printf(" %-10s |", table->schema[i].name);
        }
        printf("\n+----+");
        for (int i = 0; i < table->column_count; i++)
        {
          printf("------------+");
        }
        printf("\n");

        selectRecords(table->records, table->schema, table->column_count);

        printf("+----+");
        for (int i = 0; i < table->column_count; i++)
        {
          printf("------------+");
        }
        printf("\n");
      }
      else
      {
        printf("Erreur : Table '%s' non trouvée.\n", tableName);
      }
      break;
    case (STATEMENT_DELETE):
      //TODO implement the command here
      break;
  }
}

void command(void) {
  printf("Welcome to DatabaseProject, please type .help to get command list !\n");
  Table *database = NULL;
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",
               input_buffer->buffer);
        continue;
    }
    execute_statement(&statement, &database, input_buffer);
    printf("Executed.\n");
  }
}
