#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <stdbool.h>
#include "dynamic_string.h"
#include "scanner.h"
#include "stack.h"
#include "DLTokenList.h"

#define TABLE_SIZE 12

typedef enum
{
    MUL_DIV,
    PLUS_MINUS_DOT,
    LT,
    GT,
    LTE,
    GTE,
    EQ,
    NEQ,
    L_BRACKET,
    R_BRACKET,
    DATA,
    DOLLAR
} prec_table_index;

typedef enum
{
    P, // <
    R, // >
    S, // =
    E, // error
    D  // done
} prec_table_job;

void precedence(DLTokenL *token_list);

prec_table_index type_to_job(int type);

bool reduce(Stack_t *stack, token *token);

bool push(Stack_t *stack, token *token);

// bool equal(Stack_t* stack, token* token);

#endif