//
// Created by xfuisl00 on 22.10.2022.
//

#include "scanner.c"
#include "dynamic_string.c"
#include "basicScannerFunctions.c"
char* tokenTypos[] = {"keyword", "identificator", "variable", "sstring", "type", "integer", "exponent", "ffloat", "add", "sub", "mul", "ddiv", "openBracket", "closeBracket", "openCurly", "closeCurly", "openSquare", "closeSquare", "equal", "cmpEqual", "notEquals", "greater", "lower", "greaterEqual", "lowerEqual", "semicolumn", "end", "declare", "prolog", "comma"};

int main(){/*
    dynamic_string string;
    initialize_string(&string);
    exit(0);*/
    token* currentToken = GetToken();
    int i = 0;
    while(true){
        printf("Token #%i Type = %s Value = %s\n", i, tokenTypos[currentToken->tokenType], currentToken->value->string);
        if(currentToken->tokenType == end)
            break;
        currentToken = GetToken();
    }
}