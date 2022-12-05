#ifndef IFJ2022_PARSER_H
#define IFJ2022_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "basicParserFunctions.c"

#define DEBUG if(flag_debug)
bool flag_debug = true;
bool flag_prolog = true;

bool inFunctionDefinition = false;
int currFunctionReturnType;

void loadFunctionDefs();
void runAnalysis();

void ifStatement();
void whileStatement();
void elseStatement();
void compoundStatement();
void functionDefinition();
void statement();
DLTokenL* consumeExpression(bool canBeEmpty, int startingRoundBracketCount);
void variableDefinition();
void functionCall();

void loadFunctionDefinition();


#endif //IFJ2022_PARSER_H