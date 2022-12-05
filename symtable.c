/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

size_t Symtable_HashKey(char *identifier) { //http://www.cse.yorku.ca/~oz/hash.html
    char* str_old = identifier;
    size_t hashedKey = 5381;
    int c;

    while ((c = *identifier++))
        hashedKey = ((hashedKey << 5) + hashedKey) + c; /* hashedKey * 33 + c */

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
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
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

Symbol* Symbol_Make(char* identifier, int dataType) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->dataType = dataType;

    char* identifierCopy = (char*)malloc(strlen(identifier));
    if (!identifierCopy) exit(99);
    strcpy(identifierCopy, identifier);
    newSymbol->identifier = identifierCopy;

    //newSymbol->identifier = (char*)malloc(identifier->);
    //strcpy(newSymbol->identifier, identifier->string);
    //dynamic_string* newIdentifier = (dynamic_string*)malloc(sizeof(dynamic_string));
	//initialize_string(newIdentifier);
	//add_str_to_string(newIdentifier, identifier->string);
   // newSymbol->identifier = newIdentifier;
	return newSymbol;
}
void Symbol_Free(Symbol* symbolToFree) {
    //free_string(symbolToFree->identifier);
    if (symbolToFree){
        if(symbolToFree->identifier) free(symbolToFree->identifier);
        free(symbolToFree);
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie Symtable_Delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
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
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou Symtable_PrintNode a bez
 * použitia vlastných pomocných funkcií.
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
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
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
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu Symtable_PrintNode.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
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
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu Symtable_PrintNode.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
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
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu Symtable_PrintNode.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
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
    "colon"
};

/*
 * Pomocná funkcia ktorá vypíše uzol stromu.
 */
void Symtable_PrintNode(Symtable_node *node) {
    if (!node || !node->symbol)
        //printf("\x1B[30m[\\\\, \\\\, \\\\]\033[0m\n");
        printf("\x1B[30m[\\\\, \\\\]\033[0m\n");
    else{
        char* type = node->symbol->dataType == -1 ? "\\\\" : smtbletokentypos[node->symbol->dataType];
        //printf("\x1B[30m[%zu, %s, %s]", node->key, node->symbol->identifier, type);
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