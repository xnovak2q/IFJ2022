#include "parser.h"

int runAnalysis(){
    tokenList = DLTokenL_Create();
    programDepth = 0;
/* 
    DLTokenL_FetchNext(tokenList);
    if (DLTokenL_GetLast(tokenList)->tokenType != prolog)
    {
       DLTokenL_Dispose(tokenList); exit(2);
    }

    DLTokenL_FetchNext(tokenList);
    if (DLTokenL_GetLast(tokenList)->tokenType != declare)
    {
        DLTokenL_Dispose(tokenList); exit(2);
    } 

    DLTokenL_FetchNext(tokenList);
    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
    {
        DLTokenL_Dispose(tokenList); exit(2);
    } */

    printf("\n\x1B[36mAt top level\033[0m\n");
    DLTokenL_FetchNext(tokenList);

    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            functionDefinition();
        else statement();
        printf("\n\x1B[36mAt top level\033[0m\n");
        DLTokenL_FetchNext(tokenList);
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

    int functionCall_bracketCount;
    while (token_is_expressionMember(DLTokenL_GetLast(tokenList)) || is_functionCall())
    {
        if (is_functionCall()){
            functionCall_bracketCount = 1;
            DLTokenL_FetchNext(tokenList); 
            DLTokenL_FetchNext(tokenList); // foo(>arg<
            fetchedTokensCount += 2;

            while ((token_is_expressionMember(DLTokenL_GetLast(tokenList)) || DLTokenL_GetLast(tokenList)->tokenType == comma) && functionCall_bracketCount > 0)
            {
                if (DLTokenL_GetLast(tokenList)->tokenType == openBracket)
                    functionCall_bracketCount++;
                else if (DLTokenL_GetLast(tokenList)->tokenType == closeBracket)
                    functionCall_bracketCount--;
                                
                DLTokenL_FetchNext(tokenList);
                fetchedTokensCount++;
            }

        } else{
            fetchedTokensCount++;
            DLTokenL_FetchNext(tokenList);
        }
    }

    //DLTokenL_FetchNext(tokenList);
    bool isLineStatement = DLTokenL_GetLast(tokenList)->tokenType == semicolumn;

    for (size_t i = 0; i < fetchedTokensCount; i++)
        DLTokenL_UnFetchNext(tokenList);
    
    return isLineStatement;
}
bool is_functionCall(){
    bool isFunctionCall = false;
    if (DLTokenL_GetLast(tokenList)->tokenType == identificator){
        isFunctionCall = DLTokenL_FetchNext(tokenList)->tokenType == openBracket;
        DLTokenL_UnFetchNext(tokenList);
    }
    return isFunctionCall;
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

//LL Grammar

void functionCall(){
    printf("\x1B[36min function call\033[0m\n");

    DLTokenL_FetchNext(tokenList); //(

    bool isLastArgument;
    bool isFirstArgument = true;
    int tokensToReturn = 1;
    int startBracketCount;

    while (DLTokenL_FetchNext(tokenList)->tokenType != closeBracket)
    {
        DLTokenL_UnFetchNext(tokenList);

        while (token_is_expressionMember(DLTokenL_FetchNext(tokenList)))
            tokensToReturn++;

        if (DLTokenL_GetLast(tokenList)->tokenType == comma)
            isLastArgument = false;
        else
            isLastArgument = true;


        while (tokensToReturn > 0){
            DLTokenL_UnFetchNext(tokenList);
            tokensToReturn--;
        }  

        if (isFirstArgument == isLastArgument)
            startBracketCount = 0;
        else if (isLastArgument)
            startBracketCount = 1;
        else if(!isLastArgument)
            startBracketCount = -1;
        isFirstArgument = false;
        
        
        DLTokenL* expressionList = consumeExpression(false, startBracketCount);
        //TODO binary_tree* expressionTree = precedencka(expressionList);
        //TODO generator_variableDeclaration(variableName, , expressionTree);
        DLTokenL_Dispose(expressionList);

        //TODO parametr n

        /* if (startRoundBracketCount == 0 && DLTokenL_GetLast(tokenList)->tokenType != comma)
            exit(2); */

        if (isLastArgument){
            DLTokenL_UnFetchNext(tokenList);
            if (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
                exit(2);
            DLTokenL_UnFetchNext(tokenList);
        }
    }
    
    programDepth++;
    //TODO generate function call;
    programDepth--;
    printf("\x1B[35mout of function call\033[0m\n");
}

void whileStatement(){
    printf("\x1B[36min while statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
    //TODO precedencka(expressionTokens);
    DLTokenL_Dispose(expressionTokens);

    compoundStatement();
    printf("\x1B[35mout of while statement\033[0m\n");
}

void ifStatement(){
    printf("\x1B[36min if statement\033[0m\n");

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // prvni '('
    
    DLTokenL* expressionTokens = consumeExpression(false, 0);
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
    programDepth++;
    
    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != closeCurly)
    {
        statement();
        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLast(tokenList)->tokenType == end)
            exit(2);   
    }

    programDepth--;
    printf("\x1B[35mout of compound statement\033[0m\n");
}


void functionDefinition(){
    //if (!is_functionDefinitionHeader() || programDepth != 0) exit(2);
    if (!is_functionDefinitionHeader()) exit(2);

    printf("\x1B[36min function definition\033[0m\n");

    if (DLTokenL_FetchNext(tokenList)->tokenType != identificator) exit(2);
    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) exit(2); // function Foo->(<-string $x, int $y): bool {}
    
    if (DLTokenL_FetchNext(tokenList)->tokenType == closeBracket)
    {
        //TODO function without parameters
    } else {
        DLTokenL_UnFetchNext(tokenList); // zpet na ->(<-
        while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
        {
            if (!token_is_type(DLTokenL_FetchNext(tokenList)))        exit(2);
            if (DLTokenL_FetchNext(tokenList)->tokenType != variable) exit(2);
            //TODO zaznamenat promennou a datatyp do noveho LF
            if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) exit(2);
        }
    }
    
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon) exit(2);
    if (!token_is_type(DLTokenL_FetchNext(tokenList))) exit(2);
    //TODO zaznamenat jmeno a datatyp funkce

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    printf("\x1B[35mout of function definition\033[0m\n");
}

void lineStatement(){
    printf("\x1B[36min line-statement\033[0m\n");

    if (is_functionCall())
        functionCall();

    DLTokenL_FetchNext(tokenList);

    printf("\x1B[35mout of line-statement\033[0m\n");
    
}

DLTokenL* consumeExpression(bool canBeEmpty, int startingRoundBracketCount){
    printf("\x1B[36min expression\033[0m\n");
    DLTokenL_Last(tokenList);

    int roundBracketCount = startingRoundBracketCount;
    int nonBracketCount = 0;

    while (token_is_expressionMember(DLTokenL_GetLast(tokenList))){
        if (DLTokenL_GetLast(tokenList)->tokenType == openBracket)
            roundBracketCount++;
        else if (DLTokenL_GetLast(tokenList)->tokenType == closeBracket)
            roundBracketCount--;
        else
            nonBracketCount++;
        
        DLTokenL_FetchNext(tokenList);
    }

   /*  printf("wtf: %s (%s)\n",DLTokenL_GetLastElement(tokenList)->previousElement->token->value->string, tokenTypos[DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType]);
    printf("wtf: %s (%s)\n",DLTokenL_GetLast(tokenList)->value->string, tokenTypos[DLTokenL_GetLast(tokenList)->tokenType]);
    printf("wtf: %s (%s)\n",DLTokenL_FetchNext(tokenList)->value->string, tokenTypos[DLTokenL_GetLast(tokenList)->tokenType]);
    DLTokenL_UnFetchNext(tokenList);
     */if (roundBracketCount != 0)
        exit(2);
    if (nonBracketCount == 0 && !canBeEmpty)
        exit(2);
    

    DLTokenL* returnedList = DLTokenL_CopyFromActive(tokenList);
    DLTokenL_DeleteLast(returnedList);
    printf("\x1B[35mout of expression\033[0m\n");
    return returnedList;
}

void variableDefinition(){
    printf("\x1B[36min variable definition\033[0m\n");

    //dynamic_string* variableName = DLTokenL_GetLast(tokenList)->value;
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);

    if (!is_lineStatement(tokenList))
        exit(2);

    DLTokenL* expressionList = consumeExpression(false, 0);
    //TODO binary_tree* expressionTree = precedencka(expressionList);
    //TODO generator_variableDeclaration(variableName, , expressionTree);
    DLTokenL_Dispose(expressionList);

    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        exit(2);

    printf("\x1B[35mout of variable definition\033[0m\n");
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