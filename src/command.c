// https://github.com/Mbahal/class_db/blob/main/src/repl.c
#include "../include/command.h"
#include "../include/storage.h"
#include "../include/database.h"
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
  const ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

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

MetaCommandResult do_meta_command(InputBuffer *input_buffer, TableNode* database) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    saveAllTables(database);
    freeTableTree(database);
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

PrepareResult prepare_statement(const InputBuffer *input_buffer,
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

void execute_statement(const Statement *statement, TableNode** database, InputBuffer *input_buffer) {
  char tableName[MAX_TABLE_NAME_LENGTH];
  int key;

  printf("Entrez le nom de la table>");
  read_input(input_buffer);
  strncpy(tableName, input_buffer->buffer, MAX_TABLE_NAME_LENGTH);

  switch (statement->type) {
    case (STATEMENT_CREATE):
      createTable(database, tableName, input_buffer);
      break;
    case (STATEMENT_INSERT):
      insertRecord(*database, tableName);
      break;
    case (STATEMENT_SELECT):
      printf("Entrez la clé à rechercher: ");
      scanf("%d", &key);
      getchar();
      selectRecord(*database, tableName, key);
      break;
    case (STATEMENT_DELETE):
      printf("Entrez la clé à supprimer: ");
      scanf("%d", &key);
      getchar(); //TODO remplacer scanf car pas adapté, mais j'ai un soucis pour gérer les int donc pour le moment je reste sur scanf pour les int
      deleteRecord(database, tableName, key);
      break;
  }
}

void command(void) {
  printf("Welcome to DatabaseProject, please type .help to get command list !\n");
  TableNode* database = NULL;
  int tableCount = 0;
  loadAllTables(&database, &tableCount);
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer, database)) {
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
