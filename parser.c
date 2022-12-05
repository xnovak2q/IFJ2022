#include "parser.h"

void loadFunctionDefs(){
    printf("\x1B[36mLoading function definitions\033[0m\n");

    DLTokenL_FetchNext(tokenList);
    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            loadFunctionDefinition();
        DLTokenL_FetchNext(tokenList);
    }

    DLTokenL_UnFetchAll(tokenList);
    printf("\x1B[35mFunction definitions loaded\033[0m\n\n");
}


void runAnalysis(){
    globalFunctionsTable = Symtable_Create();
    globalVariablesTable = Symtable_Create();
    localVariablesTable = Symtable_Create();
    currVariablesTable = globalVariablesTable;

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

    loadFunctionDefs();

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

    printf("\x1B[36mAt top level\033[0m\n");
    DLTokenL_FetchNext(tokenList);

    while (DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (is_functionDefinitionHeader())
            functionDefinition();
        else statement();
        printf("\n\x1B[36mAt top level\033[0m\n");
        DLTokenL_FetchNext(tokenList);//segfault
    }

    Symtable_Inorder(globalVariablesTable);
    Symtable_Inorder(localVariablesTable);

    Symtable_Dispose(&globalVariablesTable);
    Symtable_Dispose(&localVariablesTable);
    DLTokenL_Dispose(tokenList);
}


bool is_ifStatement()              { return DLTokenL_GetLast(tokenList)->tokenType == iff       ;}
bool is_whileStatement()           { return DLTokenL_GetLast(tokenList)->tokenType == wwhile    ;}
bool is_elseStatement()            { return DLTokenL_GetLast(tokenList)->tokenType == eelse     ;}
bool is_functionDefinitionHeader() { return DLTokenL_GetLast(tokenList)->tokenType == ffunction ;}
bool is_compoundStatement()        { return DLTokenL_GetLast(tokenList)->tokenType == openCurly ;}
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
        case concat:
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

        
        DLTokenL* expressionList = consumeExpression(false, startBracketCount);

        if (isFirstArgument && !isLastArgument)
            DLTokenL_DeleteFirst(expressionList);
        else if(!isFirstArgument && isLastArgument)
            DLTokenL_DeleteLast(expressionList);

        //TODO binary_tree* expressionTree = precedencka(expressionList);
        //TODO generator_variableDeclaration(variableName, , expressionTree);
        DLTokenL_Dispose(expressionList);

        //TODO parametr n

        isFirstArgument = false;
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
    
    currVariablesTable = localVariablesTable;

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
            int argType = DLTokenL_GetLast(tokenList)->tokenType;
            if (DLTokenL_FetchNext(tokenList)->tokenType != variable) exit(2);
            Symtable_InsertSymbol(&currVariablesTable, DLTokenL_GetLast(tokenList)->value->string, argType);
            Symtable_InsertSymbol(&currVariablesTable, DLTokenL_GetLast(tokenList)->value->string, argType);

            if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) exit(2);
        }
    }
    
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon) exit(2);
    if (!token_is_type(DLTokenL_FetchNext(tokenList))) exit(2);
    //TODO zaznamenat jmeno a datatyp funkce

    DLTokenL_FetchNext(tokenList);
    compoundStatement();
    currVariablesTable = globalVariablesTable;
    Symtable_Clear(&localVariablesTable);
    printf("\x1B[35mout of function definition\033[0m\n");
}

void loadFunctionDefinition(){
    if (!is_functionDefinitionHeader()) return;
    printf("\x1B[36mloading function definition\033[0m\n");
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != identificator) return;
    char* functionName = DLTokenL_GetLast(tokenList)->value->string;

    size_t functionArgsCount = 0;
    int* functionArgs = (int*)malloc(0);

    if(DLTokenL_FetchNext(tokenList)->tokenType != openBracket) return; // function Foo->(<-string $x, int $y): bool {}
    
    if (DLTokenL_FetchNext(tokenList)->tokenType == closeBracket)
    {
        //TODO function without parameters
    } else {
        DLTokenL_UnFetchNext(tokenList); // zpet na ->(<-
        while (DLTokenL_GetLast(tokenList)->tokenType != closeBracket)
        {
            if (!token_is_type(DLTokenL_FetchNext(tokenList)))        return;
            int argType = DLTokenL_GetLast(tokenList)->tokenType;
            functionArgsCount++;
            functionArgs = (int*)realloc(functionArgs, functionArgsCount * sizeof(int));
            functionArgs[functionArgsCount-1] = argType;

            if (DLTokenL_FetchNext(tokenList)->tokenType != variable) return;


            if (DLTokenL_FetchNext(tokenList)->tokenType != comma && DLTokenL_GetLast(tokenList)->tokenType != closeBracket) return;
        }
    }
    
    
    if (DLTokenL_FetchNext(tokenList)->tokenType != colon) return;
    if (!token_is_type(DLTokenL_FetchNext(tokenList))) return;

    int functionReturnType = DLTokenL_GetLast(tokenList)->tokenType;
    Symtable_InsertSymbol(&globalFunctionsTable, functionName, functionReturnType);
    Symtable_SetFunctionArgs(globalFunctionsTable, functionName, functionArgsCount, functionArgs);

   /*  if ( Symtable_ExistsSymbol(globalFunctionsTable, functionName) )
    {
        printf("%s: %s\n", functionName, tokenTypos[Symtable_GetType(globalFunctionsTable, functionName)]);
        printf("%zu - ", Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName));

        for (size_t i = 0; i < Symtable_GetFunctionArgsCount(globalFunctionsTable, functionName); i++)
            printf("%s ", tokenTypos[Symtable_GetFunctionArgType(globalFunctionsTable, functionName, i)]);
        
        printf("\n");
    } */
    

    DLTokenL_FetchNext(tokenList);
    if (!is_compoundStatement())
        return;
    DLTokenL_FetchNext(tokenList);
    int curlyBracketCount = 1;
    while (curlyBracketCount > 0 && DLTokenL_GetLast(tokenList)->tokenType != end)
    {
        if (DLTokenL_GetLast(tokenList)->tokenType == openCurly)
            curlyBracketCount++;
        else if (DLTokenL_GetLast(tokenList)->tokenType == closeCurly)
            curlyBracketCount--;
    }
    if (curlyBracketCount != 0)
        return;
    
    printf("\x1B[35mfunction definition %s():%s loaded\033[0m\n", functionName, tokenTypos[Symtable_GetType(globalFunctionsTable, functionName)]);
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
    DLTokenL_Print(returnedList);
    printf("\x1B[35mout of expression\033[0m\n");
    return returnedList;
}

void variableDefinition(){
    printf("\x1B[36min variable definition\033[0m\n");

    char* variableName = DLTokenL_GetLast(tokenList)->value->string;
    int variableType = 0;
    DLTokenL_FetchNext(tokenList);
    DLTokenL_FetchNext(tokenList);

    if (!is_lineStatement(tokenList))
        exit(2);

    if (is_functionCall()) {
        functionCall();
        DLTokenL_FetchNext(tokenList);
    } else {
        DLTokenL* expressionList = consumeExpression(false, 0);
        //TODO binary_tree* expressionTree = precedencka(expressionList);
        //TODO generator_variableDeclaration(variableName, , expressionTree);
        //variableType = getTypeFromPrecedenceTree(expressionTree);
        DLTokenL_Dispose(expressionList);
    }

    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
        exit(2);

    Symtable_InsertSymbol(&currVariablesTable, variableName, variableType);
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