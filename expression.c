#include "expression.h"

int prec_table[TABLE_SIZE][TABLE_SIZE] = {
    //        */|+-.|< |>|<=|>=|===|!==|(|)|var|$
    /*  *  */ {R, R, R, R, R, R, R, R, P, R, P, R},
    /* +-. */ {P, R, R, R, R, R, R, R, P, R, P, R},
    /*  <  */ {P, P, R, E, E, E, R, R, P, R, P, R},
    /*  >  */ {P, P, E, R, E, E, R, R, P, R, P, R},
    /* <=  */ {P, P, E, E, R, E, R, R, P, R, P, R},
    /* >=  */ {P, P, E, E, E, R, R, R, P, R, P, R},
    /* === */ {P, P, P, P, P, P, R, E, P, R, P, R},
    /* !== */ {P, P, P, P, P, P, E, R, P, R, P, R},
    /*  (  */ {P, P, P, P, P, P, P, P, P, S, P, R},
    /*  )  */ {R, R, R, R, R, R, R, R, E, R, E, R},
    /* var */ {R, R, R, R, R, R, R, R, E, R, E, R},
    /*  $  */ {P, P, P, P, P, P, P, P, P, E, P, D},
};

prec_table_index type_to_job(int type)
{
    switch (type)
    {
    case add:
    case sub:
    case concat:
        return PLUS_MINUS_DOT;
    case mul:
    case ddiv:
        return MUL_DIV;
    case cmpEqual:
        return EQ;
    case notEquals:
        return NEQ;
    case greater:
        return GT;
    case lower:
        return LT;
    case lowerEqual:
        return LTE;
    case greaterEqual:
        return GTE;
    case openBracket:
        return L_BRACKET;
    case closeBracket:
        return R_BRACKET;
    case integer:
    case ffloat:
    case variable:
    case nnull:
    case sstring:
        return DATA;
    default:
        return DOLLAR;
    }
}

bool push(Stack_t *stack, token *token)
{
    switch (token->tokenType)
    {
    case ffloat:
    case sstring:
    case integer:
    case nnull:
    case variable:
        if (!stack_push(stack, VAR, token, true))
            return false;
        break;

    default:
        if (!stack_push(stack, OPE, token, false))
            return false;
        break;
    }
    return true;
}

bool reduce(Stack_t *stack, token *token)
{
    if (!(stack->top->stop)){
        return false;
        }

    //  var -> E
    if (stack->top->stack_type == VAR)
    {
        printf("var -> E\n");
        stack->top->stack_type = EXP;
        // je root stromu NULL? --> INSERT NODE
        //  tvoreni stromu
        return true;
    }

    // (E) -> E
    if (stack->top->token->tokenType == closeBracket && stack->top->next->stack_type == EXP && stack->top->next->next->token->tokenType == openBracket)
    {
        printf("(E) -> E\n");
        stack_pop(stack);
        stack->top->next->stack_type = stack->top->stack_type;
        stack->top->next->stop = stack->top->stop;
        stack->top->next->token = stack->top->token;
        stack_pop(stack);

        //  tvoreni stromu
        return true;
    }

    int op1 = stack->top->stack_type;
    int op2 = stack->top->next->token->tokenType;
    int op3 = stack->top->next->next->stack_type;

    if (op1 != EXP || op3 != EXP)
        return false;

    switch (op2)
    {
    case add:
    case mul:
    case ddiv:
    case concat:
    case sub:
    case cmpEqual:
    case notEquals:
    case greaterEqual:
    case lower:
    case greater:
    case lowerEqual:
        stack_pop(stack);
        stack_pop(stack);
        return true;

    default:
        return false;
    }
}

void precedence(DLTokenL *token_list)
{
    // printf("Start precedence\n");
    Stack_t stack;
    stack_initialize(&stack);

    bool done = false;

    DLTokenL_First(token_list);
    token *exp_token;

    
    dynamic_string* eos_string = malloc (sizeof(dynamic_string));
    add_str_to_string(eos_string, "eos_string");
    token* eos = makeToken(eos_string, EOS); //$ na spodu zasobniku a listu

    if (!stack_push(&stack, EOS, eos, true))
        exit(99);

    DLTokenL_InsertLast(token_list, eos);

    int input_symbol;
    int top_symbol;

    //  while podminka konec listu
    while (DLTokenL_IsActive(token_list))
    {
        // printf("----------------------------\n");
        exp_token = DLTokenL_GetActive(token_list);
        // printf("Actual token: %s\n", exp_token->value->string);
        // printf("Top stack type: %d\n", stack.top->stack_type);
        // printf("Top stack value: %s\n", stack.top->token->value->string);

        input_symbol = exp_token->tokenType;
        // hledam TERMY a NON TERMY
        if (stack.top->stack_type != EXP)
        {
            top_symbol = stack.top->token->tokenType;
        }
        else
        {
            top_symbol = stack.top->next->token->tokenType;
        }

        // printf("Input symbol: %d\n", input_symbol);
        // printf("Top symbol: %d\n", top_symbol);

        switch (prec_table[type_to_job(top_symbol)][type_to_job(input_symbol)])
        {
        case R:
            //  redukce
            // printf("REDUKCE\n");
            if (!reduce(&stack, exp_token))
                exit(99); //    TODO co za chybu
            // printf("Prosel jsem redukci\n");
            break;
        case P:
            //  push
            // printf("PUSH\n");
            if (!push(&stack, exp_token))
                exit(99); //    TODO co za chybu
            // Ziskani noveho tokenu
            DLTokenL_Next(token_list);
            break;
        case S:
            //  = equal
            // printf("EQUAL\n");
            if (!stack_push(&stack, OPE, exp_token, true))
                exit(99); //    TODO co za chybu
            // Ziskani noveho tokenu
            DLTokenL_Next(token_list);
            break;
        case D:
            //  done
            // printf("----------------------------\n");
            // printf("End precedence\n");
            return;
        case E:
            //  error
            exit(99); //    TODO co za chybu
            break;
        }
    }

    // infix -> postfix
    // postfix -> Expression tree
    return;
};

// int main(void)
// {
//     DLTokenL* list;
//     list = DLTokenL_Create();
//     // printf("List done\n");

//     dynamic_string *op10 = malloc(sizeof(dynamic_string));
//     initialize_string(op10);
//     add_char_to_string(op10, '<');
//     token* tmp = makeToken(op10, greater);
//     DLTokenL_InsertLast(list, tmp);

//     dynamic_string *op1 = malloc(sizeof(dynamic_string));
//     initialize_string(op1);
//     add_char_to_string(op1, '>');
//     tmp = makeToken(op1, lower);
//     DLTokenL_InsertLast(list, tmp);
//     // printf("Op1 done\n");
    
//     dynamic_string *op2 = malloc(sizeof(dynamic_string));
//     initialize_string(op2);
//     add_char_to_string(op2, '+');
//     tmp = makeToken(op2, add);
//     DLTokenL_InsertLast(list, tmp);
//     // printf("Op2 done\n");
    
//     dynamic_string *op3 = malloc(sizeof(dynamic_string));
//     initialize_string(op3);
//     add_char_to_string(op3, ')');
//     tmp = makeToken(op3, closeBracket);
//     DLTokenL_InsertLast(list, tmp);
//     // printf("Op3 done\n");

//     dynamic_string *op11 = malloc(sizeof(dynamic_string));
//     initialize_string(op11);
//     add_char_to_string(op11, ')');
//     tmp = makeToken(op11, closeBracket);
//     DLTokenL_InsertLast(list, tmp);
    
//     dynamic_string *op4 = malloc(sizeof(dynamic_string));
//     initialize_string(op4);
//     add_char_to_string(op4, '*');
//     tmp = makeToken(op4, mul);
//     DLTokenL_InsertLast(list, tmp);
    
//     dynamic_string *op5 = malloc(sizeof(dynamic_string));
//     initialize_string(op5);
//     add_char_to_string(op5, '7');
//     tmp = makeToken(op5, integer);
//     DLTokenL_InsertLast(list, tmp);

//     token *node;
//     DLTokenL_First(list);

//     while (DLTokenL_IsActive(list))
//     {
//         node = DLTokenL_GetActive(list);
//         printf("%s", node->value->string);
//         DLTokenL_Next(list);
//     }
//     printf("\n");
//     printf("\n");

//     precedence(list);

//     printf("Konec Main\n");
//     return 0;
// }
