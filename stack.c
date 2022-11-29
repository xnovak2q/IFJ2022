#include "stack.h"

void stack_initialize(Stack_t* stack){
    stack->top = NULL;
};

bool stack_push(Stack_t* stack, item_type type, token* token){
    Stack_item_t* tmp = (Stack_item_t*) malloc(sizeof(Stack_item_t));

    if (!tmp) return false;
    
    tmp->next = stack->top;
    tmp->stack_type = type;
    tmp->token = token;
    return true;
};

bool stack_pop(Stack_t* stack){
    if (!(stack->top)) return false;
    Stack_item_t* tmp = stack->top;
    stack->top = tmp->next;
    return true;
};