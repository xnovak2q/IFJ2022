#include "expressionTree.h"


int main(){
    return 0;
}

struct treeNode* makeNode(struct postStack_item_t item){
    treeNode* node = (struct treeNode*) malloc(sizeof(struct treeNode));
    node->item = item;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return (node);
}
void push(treeNode* node, treeNode* head){
    if(head == NULL) {
        head = node;
    }
    else{
        node->next = head;
        head = node;
    }
}
struct treeNode* pop(struct treeNode* head){
    return NULL;
}

struct treeNode* makeTree(postStack_t* stack){
    struct postStack_item_t item = Stack_Top_Item(stack);
    struct treeNode* head = makeNode(item);
}
