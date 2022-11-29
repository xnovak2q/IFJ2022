#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <stdbool.h>
#include "dynamic_string.h"
#include "scanner.h"
#include "stack.h"
#include "DLTokenList.h"

#define TABLE_SIZE 7

//precedence table indexes
typedef enum {
    PLUS_MINUS, //0 +-
    DOT,        //1 .
    MUL_DIV,    //2 */
    L_BRACKET,  //3 (
    R_BRACKET,  //4 )
    LOGIC,      //5 <, > etc.
    OPERAND,    //6 i
    DOLLAR      //7 $
} prec_table_index;

//types for stack
typedef enum {
    EOS,        // end of stack $
    STOP,       // start of expression
    TERM,       // terminal
    NON_TERM    // non-terminal
}stack_item_type;

//items on stack
typedef struct
{
    token* token;
    stack_item_type type;
}stack_item;

void expression();

#endif