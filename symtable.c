//
// Autor: xkonec86
// Implementace tabulky symbolů metodou BVS
//

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Získání unsigned hashe dle identifikátoru symbolu
 * Zdroj: http://www.cse.yorku.ca/~oz/hash.html
 */
size_t Symtable_HashKey(char *identifier) {
    char* str_old = identifier;
    size_t hashedKey = 5381;
    int c;

    while ((c = *identifier++))
        hashedKey = ((hashedKey << 5) + hashedKey) + c; // hashedKey * 33 + c

    identifier = str_old;
    return hashedKey;
}

/*
 * Vytvoreni stromu.
 */
Symtable_node* Symtable_Create() {
    Symtable_node* newSymtable = (Symtable_node*)malloc(sizeof(Symtable_node));
    newSymtable->key = 0;
    newSymtable->left = NULL;
    newSymtable->right = NULL;
    newSymtable->symbol = NULL;
    newSymtable->functionArgs = NULL;
    newSymtable->functionArgsCount = 0;
    return newSymtable;
}

/*
 * Nájdenie uzlu v strome.
 * V prípade úspechu vráti funkcia hodnotu true, v opačnom prípade funckia vráti hodnotu false
 */
bool Symtable_ExistsSymbol(Symtable_node *tree, char* identifier) {
    size_t key = Symtable_HashKey(identifier);
    if(tree != NULL){
        if(tree->key == key)
            return true;
    
        else{
            if(tree->key > key)
                return Symtable_ExistsSymbol(tree->left, identifier);
            else
                return Symtable_ExistsSymbol(tree->right, identifier);
        }
    }
    return false;
}

/*
 * Vložení symbolu do stromu
 */
void Symtable_InsertSymbol(Symtable_node **tree, char* identifier, int dataType) {
    if((*tree) == NULL){
        (*tree) = Symtable_Create();
        (*tree)->key = Symtable_HashKey(identifier);
        (*tree)->symbol = Symbol_Make(identifier, dataType);
        (*tree)->functionArgs = NULL;
    }
    else{
        if((*tree)->key == Symtable_HashKey(identifier)){
            Symbol_Free((*tree)->symbol);
            (*tree)->symbol = Symbol_Make(identifier, dataType);
        }
        else {
            if ((*tree)->key > Symtable_HashKey(identifier)) {
                Symtable_InsertSymbol(&(*tree)->left, identifier, dataType);
            } else {
                Symtable_InsertSymbol(&(*tree)->right, identifier, dataType);
            }
        }
    }
}

/*
 * Vytvoření symbolu
 */
Symbol* Symbol_Make(char* identifier, int dataType) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->dataType = dataType;

    char* identifierCopy = (char*)malloc(strlen(identifier)+1);
    if (!identifierCopy) exit(99);
    strcpy(identifierCopy, identifier);
    newSymbol->identifier = identifierCopy;

	return newSymbol;
}
void Symbol_Free(Symbol* symbolToFree) {
    if (symbolToFree){
        if(symbolToFree->identifier) free(symbolToFree->identifier);
        free(symbolToFree);
    }
}

/*
 * Pomocná funkce, která nahradí uzel nejpravějším potomkem
 */
void Symtable_ReplaceByRightmost(Symtable_node *target, Symtable_node **tree) {
    if((*tree)->right != NULL)
        Symtable_ReplaceByRightmost(target, &((*tree)->right));

    else{
        target->symbol = (*tree)->symbol;
        target->key = (*tree)->key;
        target->functionArgs = (*tree)->functionArgs;
        Symtable_node *tmp = (*tree)->left;
        //Symbol_Free((*tree)->symbol);
        free((*tree));
        *tree = tmp;
    }
}

/*
 * Odstranění symbolu
 */
void Symtable_DeleteSymbol(Symtable_node **tree, char* identifier) {
    Symtable_node *tmp = (*tree);
    size_t key = Symtable_HashKey(identifier);
    if(tmp != NULL) {
        if (tmp->key == key) {
            if (tmp->right == NULL) {
                (*tree) = tmp->left;
                Symbol_Free(tmp->symbol);
                free(tmp->functionArgs);
                free(tmp);
            }
            else {
                if (tmp->left == NULL) {
                    (*tree) = tmp->right;
                    Symbol_Free(tmp->symbol);
                    free(tmp->functionArgs);
                    free(tmp);
                }
                else 
                    Symtable_ReplaceByRightmost(tmp, &(tmp->left));
            }
        }
        else {
            if (tmp->key > key && tmp->left != NULL)
                Symtable_DeleteSymbol(&tmp->left, identifier);
            else if (tmp->right != NULL)
                Symtable_DeleteSymbol(&tmp->right, identifier);
        }
    }
}

/*
 * Uvedení celé tabulky do stavu po inicializaci
 */
void Symtable_Clear(Symtable_node **tree) {
    if((*tree) != NULL){
        if((*tree)->left != NULL)
            Symtable_Clear(&((*tree)->left));
        
        if((*tree)->right != NULL)
            Symtable_Clear(&((*tree)->right));
        
        Symbol_Free((*tree)->symbol);
        free((*tree)->functionArgs);
        free((*tree));
    }
    (*tree) = NULL;
}

void Symtable_Dispose(Symtable_node **tree) {
    Symtable_Clear(tree);
    free((*tree));
}

/*
 * Preorder průchod stromem
 */
void Symtable_PrintPreorder(Symtable_node *tree) {
    if(tree != NULL) {
        Symtable_PrintNode(tree);
        if (tree->left != NULL)
            Symtable_PrintPreorder(tree->left);
        if (tree->right != NULL)
            Symtable_PrintPreorder(tree->right);
    }
}

/*
 * Inorder průchod stromem
 */
void Symtable_PrintInorder(Symtable_node *tree) {
    if(tree != NULL) {
        if (tree->left != NULL)
            Symtable_PrintInorder(tree->left);
        Symtable_PrintNode(tree);
        if (tree->right != NULL)
            Symtable_PrintInorder(tree->right);
    }
}
/*
 * Postorder průchod stromem
 */
void Symtable_PrintPostorder(Symtable_node *tree) {
    if(tree != NULL) {
        if (tree->left != NULL)
            Symtable_PrintPostorder(tree->left);
        if (tree->right != NULL)
            Symtable_PrintPostorder(tree->right);
        Symtable_PrintNode(tree);
    }
}

/*
 * Získání datového typu symbolu dle identifikátoru
 */
int Symtable_GetType(Symtable_node *tree, char* identifier){
    size_t key = Symtable_HashKey(identifier);
    if(tree != NULL){
        if(tree->key == key)
            return tree->symbol->dataType;
    
        else{
            if(tree->key > key)
                return Symtable_GetType(tree->left, identifier);
            else
                return Symtable_GetType(tree->right, identifier);
        }
    }
    exit(99);
}

/*
 * Získání počtu argumentů funkce dle identifikátoru funkce
 */
size_t Symtable_GetFunctionArgsCount(Symtable_node *tree, char* functionName){
    size_t key = Symtable_HashKey(functionName);
    if(tree != NULL){
        if(tree->key == key)
            return tree->functionArgsCount;
    
        else{
            if(tree->key > key)
                return Symtable_GetFunctionArgsCount(tree->left, functionName);
            else
                return Symtable_GetFunctionArgsCount(tree->right, functionName);
        }
    }
    exit(99);
}

/*
 * Získání datového typu argumentu funkce dle identifikátoru funkce a indexu argumentu
 */
int Symtable_GetFunctionArgType(Symtable_node *tree, char* functionName, size_t argIndex){
    size_t key = Symtable_HashKey(functionName);
    if(tree != NULL){
        if(tree->key == key)
            if (tree->functionArgsCount < argIndex+1)
                exit(99);
            else return tree->functionArgs[argIndex];
    
        else{
            if(tree->key > key)
                return Symtable_GetFunctionArgType(tree->left, functionName, argIndex);
            else
                return Symtable_GetFunctionArgType(tree->right, functionName, argIndex);
        }
    }
    exit(99);
}

/*
 * Nastaví datové typy argumentů funkce
 */
void Symtable_SetFunctionArgs(Symtable_node *tree, char* functionName, size_t argCount, int* argTypes){
    size_t key = Symtable_HashKey(functionName);
    if(tree != NULL){
        if(tree->key == key){
            tree->functionArgsCount = argCount;
            tree->functionArgs = argTypes;
        } else{

            if(tree->key > key)
                Symtable_SetFunctionArgs(tree->left, functionName, argCount, argTypes);
            else
                Symtable_SetFunctionArgs(tree->right, functionName, argCount, argTypes);
        }
    }
}

char* smtbletokentypos[] = {
    "iff",
    "eelse",
    "ffunction",
    "nnull",
    "rreturn",
    "wwhile", 
    "identificator", 
    "variable", 
    "sstring", 
    "typeInt", 
    "typeString", 
    "typeFloat", 
    "typeVoid",
    "nullableInt", 
    "nullableString",
    "nullableFloat",
    "nullableVoid",
    "typeAny",
    "integer", 
    "exponent", 
    "ffloat", 
    "add", 
    "sub", 
    "mul", 
    "ddiv", 
    "openBracket", 
    "closeBracket", 
    "openCurly", 
    "closeCurly", 
    "openSquare", 
    "closeSquare", 
    "equal", 
    "cmpEqual", 
    "notEquals", 
    "greater", 
    "lower", 
    "greaterEqual", 
    "lowerEqual", 
    "semicolumn",
    "concat",
    "end",
    "declare",
    "prolog",
    "comma",
    "colon",
    "typeBool"
};
/*
 * Pomocná funkce, která vypíše uzel stromu
 */
void Symtable_PrintNode(Symtable_node *node) {
    if (!node || !node->symbol)
        printf("\x1B[30m[\\\\, \\\\]\033[0m\n");
    else{
        char* type = node->symbol->dataType == -1 ? "\\\\" : smtbletokentypos[node->symbol->dataType];
        printf("\x1B[30m[%s, %s]", node->symbol->identifier, type);

        if (node->functionArgs){
            printf(": ");
            if (node->functionArgsCount == 0)
                printf("no_arguments");
            else for (size_t i = 0; i < node->functionArgsCount; i++)
                printf("%s ", smtbletokentypos[node->functionArgs[i]]);
        }

        printf("\033[0m\n");
    }
}