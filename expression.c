#include "expression.h"

prec_table_index prec_table[TABLE_SIZE][TABLE_SIZE] = {
//          */  +-.   <    >   <=    >=  ===  !==   (    )   var   $
/*  *  */ { P,   P,   P,   P,   P,   P,   P,   P,   R,   P,   R,   P},   
/* +-. */ { R,   P,   P,   P,   P,   P,   P,   P,   R,   P,   R,   P},         
/*  <  */ { R,   R,   P,   E,   E,   E,   P,   P,   R,   P,   R,   P},   
/*  >  */ { R,   R,   E,   P,   E,   E,   P,   P,   R,   P,   R,   P},   
/* <=  */ { R,   R,   E,   E,   P,   E,   P,   P,   R,   P,   R,   P},   
/* >=  */ { R,   R,   E,   E,   E,   P,   P,   P,   R,   P,   R,   P},   
/* === */ { R,   R,   R,   R,   R,   R,   P,   E,   R,   P,   R,   P},   
/* !== */ { R,   R,   R,   R,   R,   R,   E,   P,   R,   P,   R,   P},   
/*  (  */ { R,   R,   R,   R,   R,   R,   R,   R,   R,   S,   R,   P},   
/*  )  */ { P,   P,   P,   P,   P,   P,   P,   P,   E,   P,   E,   P},   
/* var */ { P,   P,   P,   P,   P,   P,   P,   P,   E,   P,   E,   P},   
/*  $  */ { R,   R,   R,   R,   R,   R,   R,   R,   R,   R,   R,   D},   
};

prec_table_index type_to_symbol(token* token){
    // ziskani informace o jake pravidlo se bude jednat podle tabulky
}



void precedence(DLTokenL* token_list){
    Stack_t stack;
    stack_initialize(&stack);

    token eos; //$ na spodu zasobniku
    if (stack_push(&stack, EOS, &eos, true)) exit(99);

    // nacist token

    // check na spravnost typu tokenu -> jinak exit (stack_free)

    // pokud je token float, string, int nebo identifikator -> push VARIABLE a stop true
    //                                                      -> jinak NOT_VARIABLE a stop false

    //  while podminka konec listu
        //  nacist dalsi token
        //  check na spravnost typu tokenu -> jinak exit (stack_free)
    return;
};
