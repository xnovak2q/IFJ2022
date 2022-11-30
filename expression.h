#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include <stdbool.h>
#include "dynamic_string.h"
#include "scanner.h"
#include "stack.h"
#include "DLTokenList.h"

#define TABLE_SIZE 12

//precedence table indexes
typedef enum {
    P,  // <
    R,  // >
    S,  // =
    E,  // error
    D   // done
} prec_table_index;

void expression();

#endif