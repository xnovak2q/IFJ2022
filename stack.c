#include "stack.h"

void stack_initialize(Stack_t *stack)
{
    stack->top = NULL;
};

bool stack_push(Stack_t *stack, item_type type, token *token, bool stop)
{
    Stack_item_t *tmp = (Stack_item_t *)malloc(sizeof(Stack_item_t));

    if (!tmp)
        return false;

    tmp->next = stack->top;
    stack->top = tmp;

    tmp->stack_type = type;
    tmp->token = token;
    tmp->stop = stop;

    return true;
};

bool stack_pop(Stack_t *stack)
{
    if (!(stack->top))
        return false;

    Stack_item_t *tmp = stack->top;
    stack->top = tmp->next;
    free(tmp);

    return true;
};

bool stack_free(Stack_t *stack)
{
    if (!(stack))
        return false;

    while (stack->top)
    {
        Stack_item_t *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }

    return true;
}