#include "parser.h"

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
        if (is_functionDefinitionHeader())
            functionDefinition();
        else statement();
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


bool is_ifStatement()               { return DLTokenL_GetLast(tokenList)->tokenType == iff       ;}
bool is_whileStatement()            { return DLTokenL_GetLast(tokenList)->tokenType == wwhile    ;}
bool is_elseStatement()             { return DLTokenL_GetLast(tokenList)->tokenType == eelse     ;}
bool is_functionDefinitionHeader()  { return DLTokenL_GetLast(tokenList)->tokenType == ffunction ;}
bool is_compoundStatement()         { return DLTokenL_GetLast(tokenList)->tokenType == openCurly ;}
bool is_variableDefinition(){
    DLTokenL_FetchNext(tokenList);
    bool isVariableDefinition = DLTokenL_GetLast(tokenList)->tokenType == equal && DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType == variable;
    DLTokenL_UnFetchNext(tokenList);
    return isVariableDefinition;
}
bool is_lineStatement(){
    int fetchedTokensCount = 0;
    while (
        DLTokenL_GetLast(tokenList)->tokenType != semicolumn  &&
        DLTokenL_GetLast(tokenList)->tokenType != end &&
        !is_ifStatement() && !is_whileStatement() && !is_elseStatement() && !is_functionDefinitionHeader && !is_compoundStatement && !is_variableDefinition
       ) 
    {
        fetchedTokensCount++;
        DLTokenL_FetchNext(tokenList);
    }

    DLTokenL_FetchNext(tokenList);
    bool isLineStatement = DLTokenL_GetLast(tokenList)->tokenType == semicolumn || (DLTokenL_GetLast(tokenList)->tokenType == end && DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType == semicolumn);

    for (size_t i = 0; i < fetchedTokensCount; i++)
        DLTokenL_UnFetchNext(tokenList);
     
    return isLineStatement;
}
bool token_is_type(token* token){
    switch (token->tokenType){
        case typeInt:
        case typeString:
        case typeFloat:
        case typeVoid:
        case nullableInt:
        case nullableString:
        case nullableFloat:
        case nullableVoid:
            return true;    
        default:
            return false;
    }
}
bool token_is_expressionMember(token* token){
    switch (token->tokenType){
        case variable:
        case sstring:
        case integer:
        case ffloat:
        case exponent:
        case add:
        case sub:
        case mul:
        case ddiv:
        case openBracket:
        case closeBracket:
        case cmpEqual:
        case notEquals:
        case greater:
        case lower:
        case greaterEqual:
        case lowerEqual:
        case nnull:
            return true;
        default:
            return false;
    }
}

void whileStatement(){
    printf("\x1B[36min while statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression();
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();
    printf("\x1B[35mout of while statement\033[0m\n");
}

void ifStatement(){
    printf("\x1B[36min if statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression();
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

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


void functionDefinition(){
    if (!is_functionDefinitionHeader()) exit(2);

    printf("\x1B[36min function definition\033[0m\n");

    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList); // function Foo->(<-string $x, int $y): bool {}
    
    while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
    {
        if (!token_is_type(DLTokenL_FetchNext(tokenList)))        exit(2);
        if (DLTokenL_FetchNext(tokenList)->tokenType != variable) exit(2);
        //TODO zaznamenat promennou a datatyp do noveho LF
        if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) exit(2);
    }
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon || !token_is_type(DLTokenL_FetchNext(tokenList))) exit(2);
    //TODO zaznamenat jmeno a datatyp funkce

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    printf("\x1B[35mout of function definition\033[0m\n");
}

void lineStatement(){

}

void statement(){
    printf("\x1B[36min statement\033[0m\n");
    if(is_ifStatement())   
        ifStatement();
    else if (is_whileStatement())
        whileStatement();
    else if (is_compoundStatement())
       compoundStatement();
    else if (is_variableDefinition())
       variableDefinition();
    else if (is_lineStatement())
       lineStatement();
    else
        exit(2);

    printf("\x1B[35mout of statement\033[0m\n");
}

DLTokenL* consumeExpression(){
    DLTokenL_Last(tokenList);

    int roundBracketCount = 0;
    while (token_is_expressionMember(DLTokenL_GetLast(tokenList))){
        if (DLTokenL_GetLast(tokenList)->tokenType == openBracket)
            roundBracketCount++;
        else if (DLTokenL_GetLast(tokenList)->tokenType == closeBracket)
            roundBracketCount--;

        DLTokenL_FetchNext(tokenList);
    }

    if (roundBracketCount != 0)
        exit(2);

    DLTokenL_UnFetchNext(tokenList);
    DLTokenL* returnedList = DLTokenL_CopyFromActive(tokenList);
    DLTokenL_FetchNext(tokenList); //pro udrzeni konvence, ze kazda dalsi funkce se jiz nachazi na zacatku sve odpovidajici sekvence
    return returnedList;
}

void variableDefinition(){
    printf("\x1B[36min variable definition\033[0m\n");

    dynamic_string* variableName = DLTokenL_GetLast(tokenList)->value;
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);
    if (!is_lineStatement(tokenList))
        exit(2);

    DLTokenL* expressionList = consumeExpression();
    //TODO expression = precedencka(expressionList);
    //TODO generovaniPromenne(variableName, expression);
    DLTokenL_Dispose(expressionList);

    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        exit(2);

    DLTokenL_FetchNext(tokenList);

    printf("\x1B[35mout of variable definition\033[0m\n");
}