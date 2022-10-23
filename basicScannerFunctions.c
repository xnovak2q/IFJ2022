//
// Created by xfuisl00 on 15.10.2022
//

#include "basicScannerFunctions.h"

const char *keywords[] = {
        "else", "function", "if", "null", "return", "while"
};

const char *types[] = {
        "float", "int", "string", "void"
};

bool isLetterUnder(char x){
    return isalpha(x) || x == '_';
}

bool isLetterUnderNumber(char x){
    return isLetterUnder(x) || isdigit(x);
}
bool isKeyword(dynamic_string *x){
    for(int i = 0; i < sizeof keywords; i++){
        if(!strcmp(x->string, keywords[i]))
            return true;
    }
    return false;
}
bool isQuot(char x){
    return x =='"';
}
bool isBackslash(char x){
    return x == '\\';
}
bool isDot(char x){
    return x == '.';
}
bool isE(char x){
    return x == 'E' || x == 'e';
}
bool isMinus(char x){
    return x == '-';
}
bool isOpenBracket(char x){
    return x == '[';
}
bool isCloseBracket(char x){
    return x == ']';
}
bool isMinusPlus(char x){
    return isMinus(x) || x == '+';
}
bool isOperator(char x){
    switch (x) {
        case '#':
        case '*':
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
        case '>':
        case '<':
        case '~':
        case ',':
            return true;
        default:
            return isMinusPlus(x) || isEqual(x) || isSlash(x);
    }
}
bool isEqual(char x){
    return x == '=';
}
bool isSlash(char x){
    return x == '/';
}
bool isEOF(char x){
    return x == EOF;
}

void skipLineComment(){
    char x = getchar();
    while(x != '\n'){
        x = getchar();
    }
}

void skipBlockComment(){
    char x = getchar();
    while (true){
        if(isStar(x)){
            x = getchar();
            if(isSlash(x))
                break;
        }
    }
}

bool isLine(char x){
    return x == '\n';
}

bool isStar(char x){
    return x == '*';
}

bool isQuestion(char x){
    return x == '?';
}

bool isType(dynamic_string *x){
    for(int i = 0; i < sizeof types; i++){
        if(!strcmp(x->string, types[i]))
            return true;
    }
    return false;
}

bool isDollar(char x){
    return x == '$';
}

bool isValidText(char x){
    return isOperator(x) || isspace(x);
}

bool isValidOper(char x){
    return isLetterUnder(x) || isdigit(x) || isDollar(x) || x == ' ';
}