#ifndef IFJ2022_EXPRESSIONTREE_H
#define IFJ2022_EXPRESSIONTREE_H

#include "scanner.h"
#include "postfixStack.h"

typedef struct{
    struct postStack_item_t* item;
    struct treeNode* left;
    struct treeNode* right;
}treeNode;

treeNode* makeNode(postStack_item_t* item);
void push(treeNode* node, treeNode* head);
treeNode* pop(treeNode* head);
treeNode* makeTree(postStack_t* stack);




#endif //IFJ2022_EXPRESSIONTREE_H
