#include <stdbool.h>

#define _GNU_SOURCE
#define _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

typedef enum
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum
{
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatemantType;

typedef struct
{
    StatemantType type;
} Statement;

typedef enum
{
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef struct
{
    char *buffer;
    size_t tamanho_do_buffer;
    ssize_t tamanho_do_input;
} TypeInputBuffer;

TypeInputBuffer *novo_input_buffer()
{
    TypeInputBuffer *input_buffer = malloc(sizeof(TypeInputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->tamanho_do_buffer = 0;
    input_buffer->tamanho_do_input = 0;

    return input_buffer;
}

MetaCommandResult do_meta_command(TypeInputBuffer *input_buffer)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(TypeInputBuffer *input_buffer, Statement *statemaent)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    {
        statemaent->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0)
    {
        statemaent->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
    switch (statement->type)
    {
    case (STATEMENT_INSERT):
        printf("This is where we would do an insert.\n");
        break;
    case (STATEMENT_SELECT):
        printf("This is where we would do a select.\n");
        break;
    }
}

void print_prompt()
{
    printf("db > ");
}

void ler_input(TypeInputBuffer *input_buffer)
{
    ssize_t ler_bytes = getline(&(input_buffer->buffer), &(input_buffer->tamanho_do_buffer), stdin);

    if (ler_bytes <= 0)
    {
        printf("Error reading input \n");
        exit(EXIT_FAILURE);
    }

    // ignorar nova linha a direita

    input_buffer->tamanho_do_input = ler_bytes - 1;
    input_buffer->buffer[ler_bytes - 1] = 0;
}

void limpar_input_buffer(TypeInputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char *argv[])
{
    TypeInputBuffer *input_buffer = novo_input_buffer();

    while (true)
    {
        print_prompt();
        ler_input(input_buffer);

        if (input_buffer->buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",
                   input_buffer->buffer);
            continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}
