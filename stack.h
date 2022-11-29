#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "expression.h"

typedef enum
{
    EOS,
    START,
    TERM,
    NONTERM
}item_type;


//stack for precedence analysis
typedef struct stack_item
{
    token* token;
    item_type stack_type;
    struct stackitem *next;
}Stack_item_t;

typedef struct stack
{
    Stack_item_t *top;
}Stack_t;

void stack_initialize(Stack_t* stack);

bool stack_push(Stack_t* stack, item_type type, token* token);

bool stack_pop(Stack_t* stack);

#endif