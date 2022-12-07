#include "expressionTree.h"

treeNode* makeNode(struct DLTokenLElement * item){
    treeNode* node = (treeNode*) malloc(sizeof(treeNode));
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    return node;
}

treeNode* makeTree(DLTokenL * list){
    struct DLTokenLElement* item;
    DLTokenL_Last(list);
    item = DLTokenL_GetActiveElement(list);
    DLTokenL_Previous(list);
    treeNode* head = makeNode(item);
    int type = item->token->tokenType;
    if(type == sstring || type == ffloat || type == integer || type == exponent || type == variable){
        return head;
    }
    else{
        head->right = (struct Node*) makeTree(list);
        head->left = (struct Node*) makeTree(list);
        return head;
    }
}

#define COUNT 10

void print2DUtil(treeNode* root, int space){
    if(root == NULL)
        return;
    space += COUNT;
    print2DUtil(root->right, space);
    printf("\n");
    for(int i = COUNT;i < space; i++){
        printf(" ");
    }
    printf("%s", root->item->token->value->string);
    print2DUtil(root->left, space);
}
