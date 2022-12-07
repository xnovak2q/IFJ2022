//
// Autor: xnovak2q
//

#ifndef _DYNAMIC_STRING_H
#define _DYNAMIC_STRING_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define DYNSTR_LEN 8

typedef struct dynamic_string
{
    char* string;
    int string_length;
    int allocated_length;
} dynamic_string;

//incializace retezce
//delka je 1 a na konec retezce je vlozen ukoncovaci symbol
void initialize_string(dynamic_string* string);

//uvolneni pameti pouzivane dynamickym retezcem
void free_string(dynamic_string* string);

//vlozeni jednoho znaku do retezce a zvetseni jeho delky jestli je potreba
void add_char_to_string(dynamic_string* string, char character);

//pridani noveho retezce do aktualniho retezce a zvetseni jeho alokovane pameti
void add_str_to_string(dynamic_string* string, char* new_string);

#endif // _DYNAMIC_STRING_H