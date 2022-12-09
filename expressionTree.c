/*
 * Autor: xfuisl00
 * Funkce pro přetvoření double linked listu tokenů výrazu na strom výrazu
*/

#include "expressionTree.h"

/*
 * Funkce vytvářející nodu stromu s hodnotou item
*/
treeNode* makeNode(struct DLTokenLElement * item){
    treeNode* node = (treeNode*) malloc(sizeof(treeNode));
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
 * Rekurzivní funkce vracející kořenovou nodu stromu výrazu
*/
treeNode* makeTree(DLTokenL * list){
    struct DLTokenLElement* item = malloc(sizeof(struct DLTokenLElement));
    item = DLTokenL_GetActiveElement(list);
    treeNode* head = makeNode(item);
    int type = item->token->tokenType;
    if(type == sstring || type == ffloat || type == integer || type == variable || type == nnull){
        DLTokenL_Previous(list);
        return head;
    }
    else{
        DLTokenL_Previous(list);
        if(!DLTokenL_IsActive(list))
            return NULL;
        head->right = (struct Node*) makeTree(list);
        head->left = (struct Node*) makeTree(list);
        return head;
    }
}

/*
 * Funkce vracející true pokud je token typu integer, sstring nebo ffloat
*/
bool expressionTree_token_is_literal(token* token){
    switch (token->tokenType){
        case integer:
        case sstring:
        case ffloat:
            return true;    
        default:
            return false;
    }
}

/*
 * Funkce vracející true pokud má token typ operátoru
*/
bool expressionTree_token_is_operator(token* token){
    switch (token->tokenType){
        case concat:
        case add:
        case sub:
        case mul:
        case ddiv:
        case cmpEqual:
        case notEquals:
        case greater:
        case lower:
        case greaterEqual:
        case lowerEqual:
        case typeString:
        case typeFloat:
            return true;    
        default:
            return false;
    }
}

/*
 * Funkce vracející hodnotu podle typu operátorového tokenu
*/
int expressionTree_token_operator_generalReturnType(token* token){
    switch (token->tokenType){
        case add:
        case sub:
        case mul:
        case ddiv:
            return numeric;
        case concat:
            return string;
        case cmpEqual:
        case notEquals:
        case greater:
        case lower:
        case greaterEqual:
        case lowerEqual:
            return boolean;
        default:
            return invalid;
    }
}

/*
 * Funkce vracející hodnotu
*/
int expressionTree_generalType(int dataType){
    switch (dataType){
        case typeInt:
        case typeFloat:
        case nullableInt:
        case nullableFloat:
        case integer:
        case ffloat:
            return numeric;
        case typeString:
        case nullableString:
        case sstring:
            return string;
        case typeBool:
            return boolean;
        default:
            return invalid;
    }
}