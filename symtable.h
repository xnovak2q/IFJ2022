//
// Autor: xkonec86
// Implementace tabulky symbolů metodou BVS
//

#ifndef IFJ2022_SYMTABLE_H
#define IFJ2022_SYMTABLE_H

#include <stdbool.h>
#include <stdlib.h>

// Symbol
typedef struct {
    int dataType;                  // datový typ
    char* identifier;              // identifikátor
} Symbol;

// Uzel stromu
typedef struct Symtable_node_t {
  size_t key;                      // klíč
  Symbol* symbol;                  // obsah
  size_t functionArgsCount;        // počet n typů argumentu, pro tabulku funkcí
  int* functionArgs;               // uspořádaná n-tice datových typů argumentu, pro tabulku funkcí
  struct Symtable_node_t *left;    // levý potomek
  struct Symtable_node_t *right;   // pravý potomek
} Symtable_node;

size_t Symtable_HashKey(char *identifier);
Symtable_node* Symtable_Create();

void Symtable_Clear(Symtable_node **tree);
void Symtable_Dispose(Symtable_node **tree);

void Symtable_InsertSymbol(Symtable_node **tree, char* identifier, int dataType);
bool Symtable_ExistsSymbol(Symtable_node *tree, char* identifier);
void Symtable_DeleteSymbol(Symtable_node **tree, char* identifier);

void Symtable_PrintPreorder(Symtable_node *tree);
void Symtable_PrintInorder(Symtable_node *tree);
void Symtable_PrintPostorder(Symtable_node *tree);

void Symtable_ReplaceByRightmost(Symtable_node *target, Symtable_node **tree);

void Symtable_PrintNode(Symtable_node *node);
Symbol* Symbol_Make(char* identifier, int dataType);
void Symbol_Free(Symbol* symbolToFree);

int Symtable_GetType(Symtable_node *tree, char* identifier);
size_t Symtable_GetFunctionArgsCount(Symtable_node *tree, char* functionName);
int Symtable_GetFunctionArgType(Symtable_node *tree, char* functionName, size_t argIndex);
void Symtable_SetFunctionArgs(Symtable_node *tree, char* functionName, size_t argCount, int* argTypes);

#endif //IFJ2022_SYMTABLE_H