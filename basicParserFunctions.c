#include "basicParserFunctions.h"

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