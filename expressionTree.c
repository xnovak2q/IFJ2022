#include "expressionTree.h"

treeNode* makeNode(postStack_item_t* item){
    treeNode* node = (treeNode*) malloc(sizeof(treeNode));
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

treeNode* makeTree(postStack_t* stack){
    struct postStack_item* item;
    item = Stack_Top_Item(stack);
    treeNode* head = makeNode(item);
    int type = item->token->tokenType;
    if(type == sstring || type == ffloat || type == integer || type == exponent || type == variable){
        return head;
    }
    else{
        head->right = (struct treeNode*) makeTree(stack);
        head->left = (struct treeNode*) makeTree(stack);
        return head;
    }
}
