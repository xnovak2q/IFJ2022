#include "expression.h"

prec_table_index prec_table[TABLE_SIZE][TABLE_SIZE] = {
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
    /*  $  */ {R, R, R, R, R, R, R, R, R, R, R, D},
};

prec_table_index type_to_symbol(token *token)
{
    // ziskani informace o jake pravidlo se bude jednat podle tabulky
}

// nemusi byt, Marek uz kontroluje
bool valid_type(token *token)
{
    switch (token->tokenType)
    {
    case typeInt:
    case typeString:
    case typeFloat:
    case variable:
    case add:
    case mul:
    case sub:
    case ddiv:
    case cmpEqual:
    case notEquals:
    case greater:
    case lower:
    case greaterEqual:
    case lowerEqual:
        return true;
    default:
        return false;
    }
}

void precedence(DLTokenL *token_list)
{
    Stack_t stack;
    stack_initialize(&stack);

    bool done = false;

    DLTokenL_First(token_list);
    token *tmp;
    tmp = token_list->activeElement->token;

    token eos; //$ na spodu zasobniku
    if (stack_push(&stack, EOS, &eos, true))
        exit(99);

    //  while podminka konec listu
    while (!done)
    {
        if (!valid_type(token_list->activeElement->token))
            exit(99); // IDK co za chybu

        // hlavni prace

        // Ziskani noveho tokenu
        // IS IT OK?
        DLTokenL_Next(token_list);
        tmp = DLTokenL_GetActive(token_list);
        if (!tmp)
            done = true;
    }
    return;
};
