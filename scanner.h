//
// Created by Lakluk on 15.10.2022.
//

#ifndef IFJ2022_SCANNER_H
#define IFJ2022_SCANNER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "dynamic_string.h"
#include "basicScannerFunctions.h"

enum tokenType {keyword, identificator, variable, string, type, integer, exponent, ffloat, ooperator};

typedef struct{
    int tokenType;
    dynamic_string *value;
} token;

int Scanner();
void getHeader();
void getDeclare();
token* Type();
token* Word();
token* Variable();
token* Number();
token* Exponent(token* tok);
token* Float(token* tok);
token* String();
#endif //IFJ2022_SCANNER_H
