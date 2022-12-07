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
void push(treeNode* node, treeNode* head);
treeNode* pop(treeNode* head);
treeNode* makeTree(DLTokenL * list);
void print2DUtil(treeNode* root, int space);




#endif //IFJ2022_EXPRESSIONTREE_H
