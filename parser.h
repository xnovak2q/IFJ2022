#ifndef IFJ2022_PARSER_H
#define IFJ2022_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "scanner.c"
#include "dynamic_string.h"
#include "basicScannerFunctions.h"


typedef struct{
    enum ddataType{floatt, intt, stringg} dataType;
    dynamic_string* identifier;
    dynamic_string* value;
} var;



#endif //IFJ2022_PARSER_H