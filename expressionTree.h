#ifndef IFJ2022_EXPRESSIONTREE_H
#define IFJ2022_EXPRESSIONTREE_H

#include "scanner.h"
#include "postfixStack.h"

struct treeNode{
    struct postStack_item_t* item;
    struct treeNode* left;
    struct treeNode* right;
    struct treeNode* next;
};

struct treeNode* makeNode(struct postStack_item_t* item);
void push(struct treeNode* node, struct treeNode* head);
struct treeNode* pop(struct treeNode* head);
struct treeNode* makeTree(struct postStack_t* stack);




#endif //IFJ2022_EXPRESSIONTREE_H
