#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <stdbool.h>
#include <dynamic_string.h>
#include <scanner.h>

#define TABLE_SIZE 7

//precedence table indexes
typedef enum {
    PLUS_MINUS, // +-
    MUL_DIV,    // */
    L_BRACKET,  // (
    R_BRACKET,  // )
    LOGIC,      // <, > etc.
    OPERAND,    // i
    DOLLAR      // $
} prec_table_index;

//types for stack
typedef enum {
    EOS,        // end of stack $
    START,      // start of expression
    TERM,       // terminal
    NON_TERM    // non-terminal
}stack_item_type;

//items on stack
typedef struct
{
    token* token;
    stack_item_type type;
}stack_item;

void expression()

#endif