#ifndef IFJ2022_BASICPARSERFUNCTIONS_H
#define IFJ2022_BASICPARSERFUNCTIONS_H
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

bool is_ifStatement();
bool is_whileStatement();
bool is_elseStatement();
bool is_functionDefinitionHeader();
bool is_compoundStatement();
bool is_variableDefinition();
bool is_lineStatement();
bool is_functionCall();
bool token_is_type(token* token);
bool token_is_expressionMember(token* token);

#endif //IFJ2022_BASICPARSERFUNCTIONS_H