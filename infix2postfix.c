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
        return l_bracket_p;
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
    //printf("Dostal jsem tento token: %s\n", tok->value->string);
    // a+7*b
    // a7b*+
    if (Stack_IsEmpty(stack))
    {
        //printf("Stack empty\n");
        Stack_Push(stack, tok);
        return;
    }
    
    token *tmp = Stack_Top(stack);
    //printf("Toto je na topu: %s\n", tmp->value->string);
    postTypes stack_type = get_operator(tmp);
    postTypes input_type = get_operator(tok);
    if ((stack_type == l_bracket_p) ||
        ((stack_type == plus_minus_dot_p) && (input_type == mul_div_p)) ||
        ((stack_type == rel_ops_p) && (input_type == mul_div_p || input_type == plus_minus_dot_p)) ||
        ((stack_type == eq_neq_p) && (input_type != eq_neq_p)))
    {
        //printf("stack push v podmince\n");
        Stack_Push(stack, tok);
        return;
    }
    //printf("Jsem za podminkou\n");
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

    DLTokenL_First(infixExpression); //asi neni potreba, ale pro jistotu
    DLTokenL_First(new_list); //asi neni potreba, ale pro jistotu
    
    token *tok;
    
    while (DLTokenL_IsActive(infixExpression))
    {
        tok = DLTokenL_GetActive(infixExpression);

        if (tok->tokenType == openBracket)
        {
            //printf("processing open bracket\n");
            Stack_Push(stack, tok); //prisla otevrena zavorka, pushuju na stack a ctu dal
        }
        else if (tok->tokenType == closeBracket)
        {
            //printf("processing close bracket\n");
            untilLeftPar(stack, new_list); //dosla ukoncujici zavorka, zpracovavam vnitrek zavorky
        }
        else if (check_operand(tok))
        {
            //printf("processing operand\n");
            DLTokenL_InsertLast(new_list, tok); //vytvoreni posledniho prvku
        }
        else if (check_operator(tok))
        {
            DLTokenL_Print(new_list);
           // printf("processing operator\n");
            doOperation(stack, tok, new_list);
        }

        DLTokenL_Next(infixExpression);
    }
    free(stack);
    return new_list;
}

void main(void){

    DLTokenL* list;
    list = DLTokenL_Create();
    // printf("List done\n");

    dynamic_string *op10 = malloc(sizeof(dynamic_string));
    initialize_string(op10);
    add_char_to_string(op10, 'a');
    token* tmp = makeToken(op10, variable);
    DLTokenL_InsertLast(list, tmp);

    dynamic_string *op1 = malloc(sizeof(dynamic_string));
    initialize_string(op1);
    add_char_to_string(op1, '+');
    tmp = makeToken(op1, add);
    DLTokenL_InsertLast(list, tmp);
    // printf("Op1 done\n");

    dynamic_string *op2 = malloc(sizeof(dynamic_string));
    initialize_string(op2);
    add_char_to_string(op2, '7');
    tmp = makeToken(op2, integer);
    DLTokenL_InsertLast(list, tmp);
    // printf("Op2 done\n");

    dynamic_string *op3 = malloc(sizeof(dynamic_string));
    initialize_string(op3);
    add_char_to_string(op3, '*');
    tmp = makeToken(op3, mul);
    DLTokenL_InsertLast(list, tmp);
    // printf("Op3 done\n");

    dynamic_string *op4 = malloc(sizeof(dynamic_string));
    initialize_string(op4);
    add_char_to_string(op4, 'b');
    tmp = makeToken(op4, variable);
    DLTokenL_InsertLast(list, tmp);
    // printf("Op3 done\n");

    //printf("Po tokenech\n");
    DLTokenL* finish = DLTokenL_Create();
    finish = infix2postfix(list);
    //printf("Po listu\n");

    token* test;
    DLTokenL_First(finish);
    //printf("Po novem tokenu\n");
    while (DLTokenL_IsActive(finish))
    {
        // printf("Ve while\n");
        test = DLTokenL_GetActive(finish);
        // printf("%d", test->tokenType);
        //printf("%s", test->value->string);
        DLTokenL_Next(finish);
    }
    printf("\n");
    
    
    return;
}