#ifndef COMMAND_H
#define COMMAND_H
#include <sys/types.h>
#include "table.h"

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

MetaCommandResult do_meta_command(InputBuffer *input_buffer);

PrepareResult prepare_statement(const InputBuffer *input_buffer,
                                Statement *statement);

int getMaxId(const Record* root);

void execute_statement(const Statement *statement, Table **database, InputBuffer *input_buffer);

void command(void);

#endif //COMMAND_H
