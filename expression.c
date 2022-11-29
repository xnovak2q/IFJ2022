#include "expression.h"

char prec_table[TABLE_SIZE][TABLE_SIZE] =
{  //current token
//       +-  | */  |  (  |  )  |  r  |  i  |  $  |
       { '>' , '<' , '<' , '>' , '>' , '<' , '>' }, // | +-  // On top of stack
       { '>' , '>' , '<' , '>' , '>' , '<' , '>' }, // | */
       { '<' , '<' , '<' , '=' , '<' , '<' , 'B' }, // | (
       { '>' , '>' , 'B' , '>' , '>' , 'B' , '>' }, // | )
       { '<' , '<' , '<' , '>' , 'B' , '<' , '>' }, // | r
       { '>' , '>' , 'B' , '>' , '>' , 'B' , '>' }, // | i
       { '<' , '<' , '<' , 'B' , '<' , '<' , 'D' }  // | $
};

prec_table_index type_to_symbol(token* token){
    switch (token->tokenType)
    {
    case add:
        return;
    
    default:
        break;
    }
}

void precedence(DLTokenL* token_list){
    prec_stack_t stack;
    stack_initialize(&stack);

    prec_stack_item* top;
    prec_table_index table_index;
    
    stack_push(&stack, DOLLAR);

    bool work = true;

    while (work)
    {

    }

    return;
};

// //             {x}      {/}     {+}     {-}     {.}     {<}     {>}    {<=}     {>=}   {===}   {!==}   {(}       {)}    {int}   float}  {string} {var_id}    {$}
// /*  {x}  */ { REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {/}  */ { REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {+}  */ { PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {-}  */ { PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {.}  */ { PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {<}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {>}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /* {<=}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /* {>=}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /* {===} */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /* {!==} */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , REDUCE, REDUCE, PUSH   , REDUCE, PUSH   , PUSH   , PUSH   , PUSH   , REDUCE },
// /*  {(}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH   , EQUAL , PUSH   , PUSH   , PUSH   , PUSH   , ERROR  },
// /*  {$}  */ { PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH  , PUSH   , ERROR , PUSH   , PUSH   , PUSH   , PUSH   , ERROR  },