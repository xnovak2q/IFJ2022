#ifndef _POSTFIXSTACK_H_
#define _POSTFIXSTACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

typedef struct postStack_item
{
    token* token;
    struct postStack_item* next;
} postStack_item_t;


typedef struct 
{
    postStack_item_t* top;
} postStack_t;

void Stack_Init ( postStack_t* s );
bool Stack_IsEmpty ( postStack_t* s );
token* Stack_Top (postStack_t* s);
postStack_item_t * Stack_Top_Item(postStack_t* s);
void Stack_Pop ( postStack_t* s );
void Stack_Push ( postStack_t* s, token* c );

#endif