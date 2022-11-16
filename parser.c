#include "parser.h"
#include "DLTokenList.c"

int runAnalysis(){
    int i = 1;
    DLTokenL *tokenList = DLTokenL_Create();

    DLTokenL_FetchNext(tokenList);
    printf("Token #%i Type = %s Value = %s\n", i++, tokenTypos[DLTokenL_GetLast(tokenList)->tokenType],DLTokenL_GetLast(tokenList)->value->string);
    if (DLTokenL_GetLast(tokenList)->tokenType != prolog)
    {
       DLTokenL_Dispose(tokenList); return 2;
    }

    DLTokenL_FetchNext(tokenList);
    printf("Token #%i Type = %s Value = %s\n", i++, tokenTypos[DLTokenL_GetLast(tokenList)->tokenType],DLTokenL_GetLast(tokenList)->value->string);
    if (DLTokenL_GetLast(tokenList)->tokenType != declare)
    {
        DLTokenL_Dispose(tokenList); return 2;
    }

    DLTokenL_FetchNext(tokenList);
    printf("Token #%i Type = %s Value = %s\n", i++, tokenTypos[DLTokenL_GetLast(tokenList)->tokenType],DLTokenL_GetLast(tokenList)->value->string);
    if (DLTokenL_GetLast(tokenList)->tokenType != semicolumn)
    {
        DLTokenL_Dispose(tokenList); return 2;
    }

    

    while (strcmp(DLTokenL_GetLast(tokenList)->value->string, "konec"))
    {

        DLTokenL_FetchNext(tokenList);
        if (DLTokenL_GetLastElement(tokenList)->previousElement->token->tokenType == variable && DLTokenL_GetLast(tokenList)->tokenType == equal)
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
        }
        


        
    }
    
    /* while(true)
    {
        //keyword, identificator, variable, sstring, type, integer, exponent, ffloat, add, sub, mul, ddiv, openBracket, closeBracket, openCurly, closeCurly, openSquare, closeSquare, equal, cmpEqual, notEquals, greater,lower, greaterEqual, lowerEqual, semicolumn, end, declare, prolog, comma, colon, opperator
        token = GetToken();
        
        if (token->tokenType == end) break;
        else
        {
            switch(token->tokenType){
            case keyword:
                break;
            case identificator:
                break;
            case variable:
                token = GetToken();
                if (token->tokenType == equal)
                {
                }
                
                break;

            case semicolumn:
            case sstring:
            case integer:
            case ffloat:
                break;
            }
        }
        
        
        
        printf("Token #%i Type = %s Value = %s\n", i++, tokenTypos[token->tokenType],token->value->string);
        free_token(token);
    } */

    DLTokenL_Dispose(tokenList);
    return 0;
}