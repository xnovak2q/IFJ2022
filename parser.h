//
// Autor: xkonec86
// Syntaktická a sémantická analýza
//

#ifndef IFJ2022_PARSER_H
#define IFJ2022_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "expression.h"
#include "scanner.h"
#include "symtable.h"

#define DEBUG if(flag_debug)

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


bool is_ifStatement();
bool is_whileStatement();
bool is_elseStatement();
bool is_functionDefinitionHeader();
bool is_compoundStatement();
bool is_variableDefinition();
bool is_lineStatement();
bool is_functionCall();
bool is_returnStatement();
bool token_is_type(token* token);
bool token_is_expressionMember(token* token);
bool type_is_nullable(int type);

size_t typeMatchesKey(int type);
bool types_match(int type_src, int type_dest);
int getExpressionDataType(treeNode* root);


#endif //IFJ2022_PARSER_H