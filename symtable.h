//
// Created by xkonec86 on 1.11.2022.
//

#ifndef IFJ2022_SYMTABLE_H
#define IFJ2022_SYMTABLE_H

#include <stdbool.h>

typedef struct {
    int dataType;                  // datovy typ
    char* identifier;              // identifikator
} Symbol;

// Uzol stromu
typedef struct Symtable_node_t {
  size_t key;                      // kľúč
  Symbol* symbol;                  // obsah
  size_t functionArgsCount;        // počet n typů argumentu, pro tabulku funkcí
  int* functionArgs;               // uspořádaná n-tice datových typů argumentu, pro tabulku funkcí
  struct Symtable_node_t *left;    // levy potomek
  struct Symtable_node_t *right;   // pravy potomek
} Symtable_node;

size_t Symtable_HashKey(char *identifier);
Symtable_node* Symtable_Create();

void Symtable_Clear(Symtable_node **tree);
void Symtable_Dispose(Symtable_node **tree);

void Symtable_InsertSymbol(Symtable_node **tree, char* identifier, int dataType);
bool Symtable_ExistsSymbol(Symtable_node *tree, char* identifier);
void Symtable_DeleteSymbol(Symtable_node **tree, char* identifier);

void Symtable_Preorder(Symtable_node *tree);
void Symtable_Inorder(Symtable_node *tree);
void Symtable_Postorder(Symtable_node *tree);

void Symtable_ReplaceByRightmost(Symtable_node *target, Symtable_node **tree);

void Symtable_PrintNode(Symtable_node *node);
Symbol* Symbol_Make(char* identifier, int dataType);
void Symbol_Free(Symbol* symbolToFree);

int Symtable_GetType(Symtable_node *tree, char* identifier);
size_t Symtable_GetFunctionArgsCount(Symtable_node *tree, char* functionName);
int Symtable_GetFunctionArgType(Symtable_node *tree, char* functionName, size_t argIndex);
void Symtable_SetFunctionArgs(Symtable_node *tree, char* functionName, size_t argCount, int* argTypes);

#endif //IFJ2022_SYMTABLE_H