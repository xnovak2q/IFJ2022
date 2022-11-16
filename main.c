//
// Created by xfuisl00 on 22.10.2022.
//

#include "scanner.c"
#include "dynamic_string.c"
#include "basicScannerFunctions.c"

int main(){
    int i = 1;
    token *token;
    while(1){
        token = GetToken();
        printf("Token #%i Type = %s Value = %s\n", i++, tokenTypos[token->tokenType],token->value->string);
        if(token->tokenType == end)
            break;
        free_token(token);
    }
}