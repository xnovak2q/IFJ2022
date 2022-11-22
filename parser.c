#include "parser.h"
#include "DLTokenList.c"

DLTokenL *tokenList;

int runAnalysis(){
    int i = 1;

    tokenList = DLTokenL_Create();

    DLTokenL_FetchNext(tokenList);

     /*if (DLTokenL_GetLast(tokenList)->tokenType != prolog)
    {
       DLTokenL_Dispose(tokenList); return 2;
    }

    DLTokenL_FetchNext(tokenList);
    if (DLTokenL_GetLast(tokenList)->tokenType != declare)
    {
        DLTokenL_Dispose(tokenList); return 2;
    }

    DLTokenL_FetchNext(tokenList);
    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
    {
        DLTokenL_Dispose(tokenList); return 2;
    } */



    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {

        statement();
        DLTokenL_FetchNext(tokenList);
        /* if (DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType == variable && DLTokenL_GetLast(tokenList)->tokenType == equal)
        {
            DLTokenL_FetchNext(tokenList);
            DLTokenL_Last(tokenList);
            while (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
                DLTokenL_FetchNext(tokenList);

            DLTokenL *expressionList = DLTokenL_CopyFromActive(tokenList);
            DLTokenL_DeleteLast(expressionList);
            //precedencka(expressionList);
            DLTokenL_Dispose(expressionList);
            printf("vyraz zpracovan\n");
        } */

    }


    DLTokenL_Dispose(tokenList);
    return 0;
}


bool is_ifStatement(){
    return DLTokenL_GetLast(tokenList)->tokenType == keyword && !strcmp(DLTokenL_GetLast(tokenList)->value->string, "if");
}
bool is_whileStatement(){
    return DLTokenL_GetLast(tokenList)->tokenType == keyword && !strcmp(DLTokenL_GetLast(tokenList)->value->string, "while");
}
bool is_elseStatement(){
    return DLTokenL_GetLast(tokenList)->tokenType == keyword && !strcmp(DLTokenL_GetLast(tokenList)->value->string, "else");
}
bool is_functionDefinitionHeader(){
    return DLTokenL_GetLast(tokenList)->tokenType == keyword && !strcmp(DLTokenL_GetLast(tokenList)->value->string, "function");
}
bool is_compoundStatement(){
    return DLTokenL_GetLast(tokenList)->tokenType == openCurly;
}

void whileStatement(){
    printf("\x1B[36min while statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    token* currToken; 
    DLTokenL_Last(tokenList);

    int roundBracketCount = 1;
    while (roundBracketCount > 0)
    {
        currToken = DLTokenL_FetchNext(tokenList); //while(->neco<- / 2){ ... while(neco ->/<- 2){ ...

        if (currToken->tokenType == openBracket)       roundBracketCount++;
        else if (currToken->tokenType == closeBracket) roundBracketCount--;
        else if (currToken->tokenType == end)          exit(2); //TODO podminky aby to byly jen literaly a promenne
    }

    DLTokenL* expressionTokens = DLTokenL_CopyFromActive(tokenList);
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);


    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    printf("\x1B[35mout of while statement\033[0m\n");
}

void ifStatement(){
    printf("\x1B[36min if statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    token* currToken; 
    DLTokenL_Last(tokenList);

    int roundBracketCount = 1;
    while (roundBracketCount > 0)
    {
        currToken = DLTokenL_FetchNext(tokenList); //if(->neco<- / 2){ ... if(neco ->/<- 2){ ...

        if (currToken->tokenType == openBracket)       roundBracketCount++;
        else if (currToken->tokenType == closeBracket) roundBracketCount--;
        else if (currToken->tokenType == end)          exit(2); //TODO podminky aby to byly jen literaly a promenne
        printf("roundBracketCount: %d\n", roundBracketCount);
    }

    DLTokenL* expressionTokens = DLTokenL_CopyFromActive(tokenList);
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);


    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    DLTokenL_FetchNext(tokenList);

    if (is_elseStatement())
        elseStatement();
    
    printf("\x1B[35mout of if statement\033[0m\n");
}
void elseStatement(){
    printf("\x1B[36min else statement\033[0m\n");
   
    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    printf("\x1B[35mout of else statement\033[0m\n");
}

void compoundStatement(){
    if (!is_compoundStatement())
        exit(2);
    printf("\x1B[36min compound statement\033[0m\n");
    
    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != closeCurly)
    {
        statement();
        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType == end)
            exit(2);   
    }
    printf("\x1B[35mout of compound statement\033[0m\n");
}

bool inFunctionDefinition = false;

void functionDefinition(){
    if (!is_functionDefinitionHeader()) exit(2);

    printf("\x1B[36min function definition\033[0m\n");

    if(inFunctionDefinition) exit(2);
    inFunctionDefinition = true;

    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList); // function Foo->(<-string $x, int $y): bool {}

    
    while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
    {
        if (DLTokenL_FetchNext(tokenList)->tokenType != type) exit(2);
        if (DLTokenL_FetchNext(tokenList)->tokenType != variable) exit(2);
        //TODO zaznamenat promennou a datatyp do noveho LF
        if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) exit(2);
    }
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon || DLTokenL_FetchNext(tokenList)->tokenType != type) exit(2);
    //TODO zaznamenat jmeno a datatyp funkce

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    inFunctionDefinition = false;
    printf("\x1B[35mout of function definition\033[0m\n");
}

void statement(){
    printf("\x1B[36min statement\033[0m\n");
    if(is_ifStatement())   
        ifStatement();
    else if (is_whileStatement())
        whileStatement();
    else if (is_compoundStatement())
       compoundStatement();
    else if (is_functionDefinitionHeader())
       functionDefinition();
    printf("\x1B[35mout of statement\033[0m\n");
}