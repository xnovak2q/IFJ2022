#include "expression.h"

int prec_table[TABLE_SIZE][TABLE_SIZE] = {
    //        */|+-.|< |>|<=|>=|===|!==|(|)|var|$
    /*  *  */ {P, P, P, P, P, P, P, P, R, P, R, P},
    /* +-. */ {R, P, P, P, P, P, P, P, R, P, R, P},
    /*  <  */ {R, R, P, E, E, E, P, P, R, P, R, P},
    /*  >  */ {R, R, E, P, E, E, P, P, R, P, R, P},
    /* <=  */ {R, R, E, E, P, E, P, P, R, P, R, P},
    /* >=  */ {R, R, E, E, E, P, P, P, R, P, R, P},
    /* === */ {R, R, R, R, R, R, P, E, R, P, R, P},
    /* !== */ {R, R, R, R, R, R, E, P, R, P, R, P},
    /*  (  */ {R, R, R, R, R, R, R, R, R, S, R, P},
    /*  )  */ {P, P, P, P, P, P, P, P, E, P, E, P},
    /* var */ {P, P, P, P, P, P, P, P, E, P, E, P},
    /*  $  */ {R, R, R, R, R, R, R, R, R, R, R, E},
};

prec_table_index type_to_job(int type)
{
    switch (type)
    {
    case add:
    case sub:
        // case concat:
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
    // TODO
    case integer:
    case ffloat:
    case variable:
    case nnull:
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
    if (!(stack->top->stop))
        return false;

    //  var
    if (stack->top->stack_type == VAR)
    {
        stack->top->stack_type = EXP;
        stack_pop(stack);
        //  tvoreni stromu
        return true;
    }

    // (E)
    if (stack->top->token->tokenType == openBracket && stack->top->next->stack_type == EXP && stack->top->next->next->token->tokenType == closeBracket)
    {
        // stack->top->next->next->token = stack->top->next->token;
        // stack->top->next->next->stack_type = stack->top->next->stack_type;
        // stack->top->next->next->stop = stack->top->next->stop;
        // stack->top->next->next->next = stack->top->next->next;
        stack->top->next->next = stack->top->next;
        stack_pop(stack);
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
    // case concat:
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

bool equal(Stack_t *stack, token *token)
{
    if (!stack_push(stack, OPE, token, false))
        return false;

    return true;
}

void precedence(DLTokenL *token_list)
{
    Stack_t stack;
    stack_initialize(&stack);

    bool done = false;

    DLTokenL_First(token_list);
    token *exp_token;

    token eos; //$ na spodu zasobniku
    if (stack_push(&stack, EOS, &eos, true))
        exit(99);

    int input_symbol;
    int top_symbol;

    //  while podminka konec listu
    while (DLTokenL_IsActive(token_list))
    {
        exp_token = DLTokenL_GetActive(token_list);

        input_symbol = exp_token->tokenType;
        top_symbol = stack.top->token->tokenType;

        switch (prec_table[type_to_job(top_symbol)][type_to_job(input_symbol)])
        {
        case R:
            //  redukce
            if (!reduce(&stack, exp_token))
                exit(99); //    TODO co za chybu
            break;
        case P:
            //  push
            if (!push(&stack, exp_token))
                exit(99); //    TODO co za chybu
            break;
        case S:
            //  = equal
            if (!equal(&stack, exp_token))
                exit(99); //    TODO co za chybu
            break;
        case E:
            //  error
            exit(99); //    TODO co za chybu
            break;
        }

        // Ziskani noveho tokenu
        DLTokenL_Next(token_list);
    }
    return;
};
