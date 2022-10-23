//
// Created by xfuisl00 on 15.10.2022.
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

enum tokenType {keyword, identificator, variable, string, type, integer, exponent, ffloat, add, sub, mul, ddiv, openBracket, closeBracket, openCurly, closeCurly, openSquare, closeSquare, equal, cmpEqual, notEquals, greater,lower, greaterEqual, lowerEqual, semicolumn, end, declare, prolog, comma};

typedef struct{
    int tokenType;
    dynamic_string* value;
} token;

token* GetToken();
token* Prolog(token* tok);
token* Declare(token* tok);
token* Type();
token* Word();
token* Variable();
token* Number();
token* Exponent(token* tok);
token* Float(token* tok);
token* String();
token* Operator();
token* Prolog(token* tok);
#endif //IFJ2022_SCANNER_H
