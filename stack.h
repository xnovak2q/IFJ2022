#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "expression.h"

#define STACK_SIZE 101

//stack for precedence analysis
typedef struct stack_item
{
    //type for semantic
    stack_item_type stack_type;
    struct stack_item *next;
}prec_stack_item;

typedef struct 
{
    prec_stack_item *top;
}prec_stack_t;

void stack_initialize(prec_stack_t* stack);

bool stack_push(prec_stack_t* stack, stack_item_type type);

bool stack_pop(prec_stack_t* stack);



#endif