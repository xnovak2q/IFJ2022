#ifndef IFJ2022_PARSER_H
#define IFJ2022_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "scanner.c"
#include "DLTokenList.c"
#include "dynamic_string.h"
#include "basicScannerFunctions.h"


typedef struct{
    enum ddataType{floatt, intt, stringg} dataType;
    dynamic_string* identifier;
    dynamic_string* value;
} var;

bool is_ifStatement();
bool is_whileStatement();
bool is_elseStatement();
bool is_functionDefinitionHeader();
bool is_compoundStatement();
void ifStatement();
void whileStatement();
void elseStatement();
void compoundStatement();
void functionDefinition();
void statement();
bool token_is_expressionMember(token* token);
DLTokenL* consumeExpression();
bool is_variableDefinition();
void variableDefinition();


#endif //IFJ2022_PARSER_H