#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"

typedef enum
{
    EOS,
    EXP,
    VAR,
    OPE
} item_type;

// stack for precedence analysis
typedef struct stack_item
{
    token *token;
    item_type stack_type;
    struct stack_item *next;
    bool stop;
} Stack_item_t;

typedef struct stack
{
    Stack_item_t *top;
} Stack_t;

void stack_initialize(Stack_t *stack);

bool stack_push(Stack_t *stack, item_type type, token *token, bool stop);

bool stack_pop(Stack_t *stack);

bool stack_free(Stack_t *stack);

#endif