//
// Created by xfuisl00 on 22.10.2022.
//

#include "scanner.c"
#include "dynamic_string.c"
#include "basicScannerFunctions.c"

int main(){/*
    dynamic_string string;
    initialize_string(&string);
    exit(0);*/
    token* currentToken = GetToken();
    int i = 0;
    while(true){
        printf("Token #%i Type = %i Value = %s", i, currentToken->tokenType, currentToken->value->string);
        if(currentToken->tokenType == end)
            break;
        currentToken = GetToken();
    }
}