
#include "postfixStack.h"


void Stack_Init(postStack_t *stack)
{
    if (stack == NULL)
    {
        return;
    };
    stack->top = NULL; // pokud je stack prázdný, jeho topIndex by měl mít hodnotu -1
}


bool Stack_IsEmpty(postStack_t *stack)
{
    return stack->top == NULL; // pokud je prázdný podmínka je splněna (topIndex == -1), tak vrací 1, jinak 0
}

token* Stack_Top(postStack_t *stack)
{
    if (Stack_IsEmpty(stack))
    {
        return NULL;
    }
    return stack->top->token; // podívám se na topIndex stacku a jeho adresu vložím do ukazatele
}


void Stack_Pop(postStack_t *stack)
{
    if (!Stack_IsEmpty(stack))
    {
        postStack_item_t *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}


void Stack_Push(postStack_t *stack, token* data)
{
    postStack_item_t *tmp = (postStack_item_t *)malloc(sizeof(postStack_item_t));

    tmp->next = stack->top;
    stack->top = tmp;
    tmp->token = data;
}

postStack_item_t * Stack_Top_Item(postStack_t* s){
    if (Stack_IsEmpty(stack))
    {
        return NULL;
    }
    return &(stack->top);
}

/* Konec postfixStack.c */
