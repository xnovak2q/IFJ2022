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
#include "symtable.c"

DLTokenL* tokenList;
Symtable_node* globalVariablesTable;
Symtable_node* localVariablesTable;
Symtable_node* currVariablesTable;
Symtable_node* globalFunctionsTable;
int programDepth;

void loadFunctionDefs();
void runAnalysis();

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
DLTokenL* consumeExpression(bool canBeEmpty, int startingRoundBracketCount);
bool is_variableDefinition();
void variableDefinition();
bool is_functionCall();
void functionCall();

void loadFunctionDefinition();


#endif //IFJ2022_PARSER_H