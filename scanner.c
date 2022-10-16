#include "scanner.h"
#include "dynamic_string.h"

char input;

token* Scanner(){
    getHeader();
    input = getchar();
    while(isspace(input)){
        input = getchar();
    }
    if(input == 'd'){
        getDeclare();
    }
    bool end = false;
    while(!end){
        input = getchar();
        if(!isspace(input)){
            if(isQuestion(input)){
                return Type();
            }
            if(isLetterUnder(input)){
                return Word();
            }
            if(isDollar(input)){
                return Variable();
            }
            if(isdigit(input)){
                return Number();
            }
            if(isQuot(input)){
                return String();
            }
        }
    }
}

void getHeader(){
    while(true){
        input = getchar();
        if(input == '<'){
            input = getchar();
            if(input == '?'){
                char tmp1 = getchar();
                char tmp2 = getchar();
                char tmp3 = getchar();
                if(tmp1 != 'p' || tmp2 != 'h' || tmp3 != 'p'){
                    exit(2);
                }
                input = getchar();
                if(!isspace(input)){
                    if(isSlash(input)){
                        input = getchar();
                        if(isSlash(input)){
                            skipLineComment();
                        } else{
                            if (isStar(input)){
                                skipBlockComment();
                            }
                        }
                    }
                    else{
                        exit(2);
                    }
                }
                return;
            }
        }
    }
}

void getDeclare(){
    char *compare = "declare(strict_types=1);";
    for(int i = 0; i < sizeof compare; i++){
        if(input != compare[i]){
            exit(1);
        }
        input = getchar();
    }
}

token* makeToken(){
    token * tokenToMake = malloc(sizeof token);
    if(!tokenToMake){
        exit(99);
    }
    initialize_string(tokenToMake->value);
    return tokenToMake;
}

token* Type(){
    token* tokenToMake = makeToken();
    tokenToMake->tokenType = type;
    add_char_to_string(tokenToMake->value, input);
    input = getchar();
    while(isalpha(input)){
        add_char_to_string(tokenToMake->value, input);
        input = getchar();
    }
    if(!isType(tokenToMake->value)){
        exit(1);
    }
    if(!isValid(input)){
        exit(1);
    }
    return tokenToMake;
}

token* Word(){
    token* tokenToMake = makeToken();
    add_char_to_string(tokenToMake->value, input);
    input = getchar();
    while(isLetterUnder(input) || isdigit(input)){
        add_char_to_string(tokenToMake->value, input);
        input = getchar();
    }
    tokenToMake->tokenType = identificator;
    if(isType(tokenToMake->value)){
        tokenToMake->tokenType = type;
    }
    if(isKeyword(tokenToMake->value)){
        tokenToMake->tokenType = keyword;
    }
    if(!isValid(input)){
        exit(1);
    }
    return tokenToMake;
}

token* Variable(){
    token *tokenToMake = makeToken();
    tokenToMake->tokenType = variable;
    input = getchar();
    while(isLetterUnder(input) || isdigit(input)){
        add_char_to_string(tokenToMake->value, input);
        input = getchar();
    }
    if(!isValid(input)){
        exit(1);
    }
    return tokenToMake;
}

token* Number(){
    token *tokenToMake = makeToken();
    tokenToMake->tokenType = integer;
    add_char_to_string(tokenToMake->value, input);
    input = getchar();
    while(isdigit(input)){
        add_char_to_string(tokenToMake->value, input);
        input = getchar();
    }
    if(isDot(input)){
        add_char_to_string(tokenToMake->value, input);
        return Float(tokenToMake);
    }
    if(isE(input)){
        add_char_to_string(tokenToMake->value, input);
        return Exponent(tokenToMake);
    }
    if(!isValid(input)){
        exit(1);
    }
    return tokenToMake;
}

token* Exponent(token* tok){
    input = getchar();
    if(isMinusPlus(input)){
        add_char_to_string(tok->value, input);
        input = getchar();
    }
    while(isdigit(input)){
        add_char_to_string(tok->value, input);
        input = getchar();
    }
    if(!isValid(input)){
        exit(1);
    }
    tok->tokenType = exponent;
    return tok;
}

token* Float(token* tok){
    input = getchar();
    while(isdigit(input)){
        add_char_to_string(tok->value, input);
        input = getchar();
    }
    if(isE(input)){
        add_char_to_string(tok->value, input);
        return Exponent(tok);
    }
    if(!isValid(input)){
        exit(1);
    }
    tok->tokenType = ffloat;
    return tok;
}

token* String(){
    input = getchar();
    token *tokenToMake = makeToken();
    tokenToMake->tokenType = string;
    char prevChar;
    while(!isQuot(input) || isBackSlash(prevChar)){
        add_char_to_string(tok->value, input);
    }
}