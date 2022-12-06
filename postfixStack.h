#ifndef _POSTFIXSTACK_H_
#define _POSTFIXSTACK_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK 50

typedef struct 
{
    char array[MAX_STACK];
    int topIndex;
} postStack_t;

void Stack_Init ( postStack_t* s );
int Stack_IsEmpty ( const postStack_t* s );
int Stack_IsFull ( const postStack_t* s );
void Stack_Top ( const postStack_t* s, char* c );
void Stack_Pop ( postStack_t* s );
void Stack_Push ( postStack_t* s, char c );

#endif