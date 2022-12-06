#include "infix2postfix.h"

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
        return bracket;
    }
}

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
    if ((stack_type == bracket) ||
        ((stack_type == plus_minus_dot_p) && (input_type == mul_div_p)) ||
        ((stack_type == rel_ops_p) && (input_type == mul_div_p || input_type == plus_minus_dot_p)) ||
        ((stack_type == eq_neq_p) && (input_type != eq_neq_p)))
    {
        Stack_Push(stack, tok);
        return;
    }
    DLTokenL_InsertLast(postfixExpression, tmp);
    Stack_Pop(stack);
    doOperation(stack, tok, postfixExpression);
}

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
    DLTokenL_First(infixExpression);
    token *tok;
    while (DLTokenL_IsActive(new_list))
    {
        tok = DLTokenL_GetActive(infixExpression);
        if (tok->tokenType == openBracket)
        {
            Stack_Push(stack, tok);
        }
        else if (tok->tokenType == closeBracket)
        {
            untilLeftPar(stack, new_list);
        }
        else if (check_operand(tok))
        {
            DLTokenL_InsertLast(new_list, tok);
        }
        else if (check_operator(tok))
        {
            doOperation(stack, tok, new_list);
        }
    }
    free(stack);
}