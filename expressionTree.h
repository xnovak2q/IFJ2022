#ifndef IFJ2022_EXPRESSIONTREE_H
#define IFJ2022_EXPRESSIONTREE_H

#include "scanner.h"
#include "DLTokenList.h"

typedef struct Node{
    struct DLTokenLElement* item;
    struct Node* left;
    struct Node* right;
}treeNode;

treeNode* makeNode(DLTokenLElementPtr item);
treeNode* makeTree(DLTokenL * list);
void print2DUtil(treeNode* root, int space);
bool expressionTree_token_is_literal(token* token);

enum generalType {
    numeric,
    string,
    boolean,
    invalid
};

bool expressionTree_token_is_operator(token* token);
int expressionTree_token_operator_returnType(token* token);
int expressionTree_generalType(int dataType);




#endif //IFJ2022_EXPRESSIONTREE_H
