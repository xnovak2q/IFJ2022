
#include "postfixStack.h"

// inicializace zasobniku
void Stack_Init(postStack_t *stack)
{
    if (stack == NULL)
    {
        return;
    };
    stack->top = NULL; // pokud je stack prázdný, jeho topIndex by měl mít hodnotu -1
}

//  zjisteni jestli je zasobnik prazdny
bool Stack_IsEmpty(postStack_t *stack)
{
    return stack->top == NULL; // pokud je prázdný podmínka je splněna (topIndex == -1), tak vrací 1, jinak 0
}

//  ziskani vrchni polozky ze zasobniku
token* Stack_Top(postStack_t *stack)
{
    if (Stack_IsEmpty(stack))
    {
        return NULL;
    }
    return stack->top->token; // podívám se na topIndex stacku a jeho adresu vložím do ukazatele
}

//  smazani vrchni polozky ze zasobniku a jeji uvolneni v pameti
void Stack_Pop(postStack_t *stack)
{
    if (!Stack_IsEmpty(stack))
    {
        postStack_item_t *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

//  vlozeni na vrchol zasobniku
void Stack_Push(postStack_t *stack, token* data)
{
    postStack_item_t *tmp = (postStack_item_t *)malloc(sizeof(postStack_item_t));

    tmp->next = stack->top;
    stack->top = tmp;
    tmp->token = data;
}

/* Konec postfixStack.c */
