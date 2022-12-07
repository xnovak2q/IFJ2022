

#ifndef IFJ22_CODEGEN_H
#define IFJ22_CODEGEN_H


#include <stdio.h>
#include <string.h>

#include "dynamic_string.h"
#include "error.h"
#include "parser.c"
#include "dynamic_string.c"
#include "basicScannerFunctions.c"

#include "scanner.h"

/// @brief Prvek dvousmerne vazaneho seznamu pro reprezentaci instrukce
typedef struct instruction{
    dynamic_string inst_content;
    struct instruction *prev_inst;
    struct instruction *next_inst;
}inst_t;


/// @brief Struktura reprezentujici cilovy program
typedef struct output_code{
    inst_t *first_inst;
    inst_t *last_inst;
    int while_lvl;
}out_code;


#endif //IFJ22_CODEGEN_H