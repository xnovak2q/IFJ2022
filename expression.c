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

//  pomocna funkce pro orientaci v precedencni tabulce
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

//  funkce pro vkladani na zasobnik podle typu tokenu
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

//  funkce pro redukci podle pravidel
bool reduce(Stack_t *stack, token *token)
{
    // pri volani redukce musi byt na vrcholu zasobniku stop atribut, jinak jde o semantickou chybu
    if (!(stack->top->stop))
    {
        return false;
    }

    //  var -> E
    if (stack->top->stack_type == VAR)
    {
        stack->top->stack_type = EXP;
        return true;
    }

    // (E) -> E
    if (stack->top->token->tokenType == closeBracket && stack->top->next->stack_type == EXP && stack->top->next->next->token->tokenType == openBracket)
    {
        // nutno vyhodit vrchol ze zasobniku, pote posunout prvni prvek na druhe misto a opet vyhodit vrchol
        stack_pop(stack);
        stack->top->next->stack_type = stack->top->stack_type;
        stack->top->next->stop = stack->top->stop;
        stack->top->next->token = stack->top->token;
        stack_pop(stack);
        return true;
    }

    //  pomocne promenne pro vyuziti switche
    int op1 = stack->top->stack_type;
    int op2 = stack->top->next->token->tokenType;
    int op3 = stack->top->next->next->stack_type;

    if (op1 != EXP || op3 != EXP)
        return false;

    // E op E -> E
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

treeNode* precedence(DLTokenL *token_list)
{
    //  prvotni inicializace
    DLTokenL* newlist = DLTokenL_Create();
    Stack_t stack;
    stack_initialize(&stack);

    DLTokenL_First(token_list);
    token *exp_token;

    //$ na spodu zasobniku
    dynamic_string *eos_string = malloc(sizeof(dynamic_string));
    token *eos = makeToken(eos_string, EOS); 

    if (!stack_push(&stack, EOS, eos, true)){
        exit(99);
    }

    // vlozeni $ do listu
    DLTokenL_InsertLast(token_list, eos);

    int input_symbol;
    int top_symbol;

    //  while podminka konec listu
    while (DLTokenL_IsActive(token_list))
    {
        exp_token = DLTokenL_GetActive(token_list);

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

        switch (prec_table[type_to_job(top_symbol)][type_to_job(input_symbol)])
        {
        case R:
            //  reduce
            if (!reduce(&stack, exp_token))
                exit(2);
            break;
        case P:
            //  push
            if (!push(&stack, exp_token))
                exit(99);
            DLTokenL_Next(token_list);
            break;
        case S:
            //  equal
            if (!stack_push(&stack, OPE, exp_token, true))
                exit(99);
            // Ziskani noveho tokenu
            DLTokenL_Next(token_list);
            break;
        case D:
            //  done
            //  tvorba postfixoveho zapisu
            newlist = infix2postfix(token_list);
            treeNode* node;
            DLTokenL_Last(newlist);
            //  tvorba stromu z postfixoveho zapisu
            node = makeTree(newlist);
            return node;
        case E:
            //  error
            exit(2);
            break;
        }
    }
};