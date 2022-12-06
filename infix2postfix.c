#include "infix2postfix.h"

void untilLeftPar( postStack_t *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    char tmp;
    while (!Stack_IsEmpty(stack)) //iterujeme dokud není stack prázdný
    {
        Stack_Top(stack, &tmp); //vytáhnutí proměnné ze zásobníku
        Stack_Pop(stack);
        if (tmp == '(')
        {
            break; //ukončení funkce jakmile se narazí na "(" 
        }
        postfixExpression[(*postfixExpressionLength)++] = tmp; //přidání znaku do řetězce a poté zvýšení jeho délky
    }
}

void doOperation( postStack_t *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    if (Stack_IsEmpty(stack)){ //pokud je stack prázdný, přidáme do něj znak a dál se nic neděje (není kolize precedence)
        Stack_Push(stack, c);
        return;
    }
    char tmp;
    Stack_Top(stack, &tmp); //uložení proměnné z vrcholu stacku do pomocné proměnné
    if ((tmp == '(') || ((tmp == '+' || tmp == '-') && (c == '*' || c == '/'))){
        Stack_Push(stack, c);
        return;
    }
    postfixExpression[(*postfixExpressionLength)++] = tmp;
    Stack_Pop(stack);
    doOperation(stack, c, postfixExpression, postfixExpressionLength);
}