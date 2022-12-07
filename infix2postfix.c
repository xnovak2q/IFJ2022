#include "infix2postfix.h"

//  funkce vraci zda se jedna o operand nebo ne
bool check_operand(token *token)
{
    switch (token->tokenType)
    {
    case ffloat:
    case sstring:
    case nnull:
    case integer:
    case variable:
        return true;
    default:
        return false;
    }
}

// funkce vraci zda se jedna o operator nebo ne
bool check_operator(token *token)
{
    switch (token->tokenType)
    {
    case add:
    case sub:
    case mul:
    case ddiv:
    case concat:
    case cmpEqual:
    case notEquals:
    case lowerEqual:
    case greaterEqual:
    case greater:
    case lower:
        return true;
    default:
        return false;
    }
}

//  funkce vraci typ tokenu
postTypes get_operator(token *token)
{
    switch (token->tokenType)
    {
    case add:
    case sub:
    case concat:
        return plus_minus_dot_p;
    case mul:
    case ddiv:
        return mul_div_p;
    case cmpEqual:
    case notEquals:
        return eq_neq_p;
    case greater:
    case lower:
    case greaterEqual:
    case lowerEqual:
        return rel_ops_p;
    case openBracket:
        return l_bracket_p;
    }
}

//  zpracovani vnitrku zavorek
void untilLeftPar(postStack_t *stack, DLTokenL *postfixExpression)
{
    token *tmp;
    while (!Stack_IsEmpty(stack))
    {
        tmp = Stack_Top(stack);
        Stack_Pop(stack);
        if (tmp->tokenType == openBracket)
        {
            break;
        }
        DLTokenL_InsertLast(postfixExpression, tmp);
    }
}

//  zpracovani operatoru a jeho precedence
void doOperation(postStack_t *stack, token *tok, DLTokenL *postfixExpression)
{
    if (Stack_IsEmpty(stack))
    {
        Stack_Push(stack, tok);
        return;
    }
    
    token *tmp = Stack_Top(stack);
    postTypes stack_type = get_operator(tmp);
    postTypes input_type = get_operator(tok);

    //  zjisteni o jaky operator se jedna na zasobniku a o jaky se jedna na vstupu
    //  a rozuzleni jeho precedence
    if ((stack_type == l_bracket_p) ||
        ((stack_type == plus_minus_dot_p) && (input_type == mul_div_p)) ||
        ((stack_type == rel_ops_p) && (input_type == mul_div_p || input_type == plus_minus_dot_p)) ||
        ((stack_type == eq_neq_p) && (input_type != eq_neq_p)))
    {
        Stack_Push(stack, tok);
        return;
    }
    DLTokenL_InsertLast(postfixExpression, tmp);
    Stack_Pop(stack);
    // rekurzivni volani
    doOperation(stack, tok, postfixExpression);
}

// hlavni funkce pro konverzi mezi infix a postfix zapisem
DLTokenL *infix2postfix(DLTokenL *infixExpression)
{
    DLTokenL *new_list = DLTokenL_Create();
    if (!new_list)
        exit(99);

    postStack_t *stack = (postStack_t *)malloc(sizeof(postStack_t));
    if (!stack)
    {
        DLTokenL_Dispose(new_list);
        exit(99);
    }
    
    Stack_Init(stack);

    DLTokenL_First(infixExpression); //asi neni potreba, ale pro jistotu
    DLTokenL_First(new_list); //asi neni potreba, ale pro jistotu
    
    token *tok;
    
    //  hlavni cyklus cele konverze -> iterujeme pres vsechny polozky listu
    while (DLTokenL_IsActive(infixExpression))
    {
        tok = DLTokenL_GetActive(infixExpression);

        if (tok->tokenType == openBracket)
        {
            Stack_Push(stack, tok); //prisla otevrena zavorka, pushuju na stack a ctu dal
        }
        else if (tok->tokenType == closeBracket)
        {
            untilLeftPar(stack, new_list); //dosla ukoncujici zavorka, zpracovavam vnitrek zavorky
        }
        else if (check_operand(tok))
        {
            DLTokenL_InsertLast(new_list, tok); //vlozeni posledniho prvku
        }
        else if (check_operator(tok))
        {
            doOperation(stack, tok, new_list); //prisel operator, musi se vyresit jeho precedence
        }

        DLTokenL_Next(infixExpression);
    }

    //  cely vyraz byl zpracovan, ale operatory zustaly na zasobniku -> nutno je pridat do seznamu v postfixovem poradi
    while (!Stack_IsEmpty(stack))
    {
        DLTokenL_InsertLast(new_list, Stack_Top(stack));
        Stack_Pop(stack);
    }
    
    free(stack);
    return new_list;
}