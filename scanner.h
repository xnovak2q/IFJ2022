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

enum tokenType {
    iff,
    eelse,
    ffunction,
    nnull,
    rreturn,
    wwhile,
    identificator, 
    variable,
    sstring,
    typeInt,
    typeString,
    typeFloat,
    typeVoid,
    nullableInt,
    nullableString,
    nullableFloat,
    nullableVoid,
    integer,
    exponent,
    ffloat,
    add,
    sub,
    mul,
    ddiv,
    openBracket,
    closeBracket,
    openCurly,
    closeCurly,
    openSquare,
    closeSquare,
    equal,
    cmpEqual,
    notEquals,
    greater,
    lower,
    greaterEqual,
    lowerEqual,
    semicolumn,
    end,
    declare,
    prolog,
    comma,
    colon,
    opperator
};

typedef struct{
    int tokenType;
    dynamic_string* value;
} token;

token* GetToken();
token* Prolog();
token* Declare(dynamic_string *string);
token* Type(char input);
token* Word(char input);
token* Variable();
token* Number(char input);
token* Exponent(dynamic_string *string,char input);
token* Float(dynamic_string *string,char input);
token* String();
token* Operator(char input);
token* makeToken(dynamic_string *string, enum tokenType type);
void free_token(token* token);
#endif //IFJ2022_SCANNER_H
