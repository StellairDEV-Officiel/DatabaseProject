#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include "table_btree.h"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
  } MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_CREATE, STATEMENT_INSERT, STATEMENT_SELECT, STATEMENT_DELETE } StatementType;

typedef struct {
    StatementType type;
} Statement;

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer();

void print_prompt();

void read_input(InputBuffer *input_buffer);

void close_input_buffer(InputBuffer *input_buffer);

MetaCommandResult do_meta_command(InputBuffer *input_buffer, TableNode *database);

PrepareResult prepare_statement(const InputBuffer *input_buffer,
                                Statement *statement);

void execute_statement(const Statement *statement, TableNode** database, InputBuffer *input_buffer);

void command(void);

#endif //COMMAND_H
