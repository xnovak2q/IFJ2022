#include "scanner.h"


token* GetToken() {
    char input = getchar();
    if (isQuestion(input)) {
        return Type(input);
    }
    if (isLetterUnder(input)) {
        return Word(input);
    }
    if (isDollar(input)) {
        return Variable();
    }
    if (isdigit(input)) {
        return Number(input);
    }
    if (isQuot(input)) {
        return String();
    }
    if (isOperator(input)) {
        return Operator(input);
    }
}

token* Prolog() {
    char tmp1 = getchar();
    char tmp2 = getchar();
    char tmp3 = getchar();
    if (tmp1 != 'p' || tmp2 != 'h' || tmp3 != 'p') {
        exit(2);
    }
    char input = getchar();
    if (!isspace(input)) {
        if (isSlash(input)) {
            input = getchar();
            if (isSlash(input)) {
                skipLineComment();
            } else {
                if (isStar(input)) {
                    skipBlockComment();
                }
            }
        } else {
            exit(2);
        }
    }
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_str_to_string(string, "<?php");
    ungetc(input, stdin);
    return makeToken(string, prolog);
}

token* Declare(dynamic_string* string){
    char input = getchar();
    if(!isOpenBracket(input)){
        exit(1);
    }
    free_string(string);
    initialize_string(string);
    input = getchar();
    while(!isCloseBracket(input)){
        add_char_to_string(string,input);
        input = getchar();
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string,declare);
}

token* makeToken(dynamic_string* string, enum tokenType type){
    token * tokenToMake = (token*)malloc(sizeof (token));
    if(!tokenToMake){
        exit(99);
    }
    tokenToMake->value = string;
    tokenToMake->tokenType = type;
    printf("madeToken %i\n", type);
    return tokenToMake;
}

token* Type(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    if(input == '>'){
        return makeToken(string, end);
    }
    while(isalpha(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(!isType(*string)){
        exit(1);
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, type);
}

token* Word(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    printf("%c\n", input);
    while(isLetterUnder(input) || isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
        printf("%c\n", input);
    }
    if(strcmp(string->string, "declare") == 0){
        return Declare(string);
    }
    if(!isValidText(input)){
        exit(1);
    }
    if(isKeyword(*string)){
        return makeToken(string, keyword);
    }
    if(isType(*string)){
        return makeToken(string,type);
    }
    ungetc(input, stdin);
    return makeToken(string, identificator);
}

token* Variable(){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    char input = getchar();
    while(isLetterUnder(input) || isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, variable);
}

token* Number(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    while(isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(isDot(input)){
        add_char_to_string(string, input);
        return Float(string,input);
    }
    if(isE(input)){
        add_char_to_string(string, input);
        return Exponent(string,input);
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, integer);
}

token* Exponent(dynamic_string* string, char input){
    input = getchar();
    if(isMinusPlus(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    while(isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, exponent);
}

token* Float(dynamic_string* string, char input){
    input = getchar();
    while(isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(isE(input)){
        add_char_to_string(string, input);
        return Exponent(string,input);
    }
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, ffloat);
}

token* String(){
    char input = getchar();
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    char prevChar;
    while(!isQuot(input) || isBackslash(prevChar)){
        //TODO escape sekvence
        if(isBackslash(input)){

        }
        add_char_to_string(string, input);
    }
    input = getchar();
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, sstring);
}

token* Operator(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    token *tokenToMake = makeToken(string, -1);
    char nextChar = getchar();
    switch (input) {
        case '<':
            if(nextChar == '?'){
                return Prolog();
            }
            if(nextChar == '=') {
                tokenToMake->tokenType = lowerEqual;
                add_str_to_string(string, "<=");
            }
            else{
                tokenToMake->tokenType = lower;
                add_char_to_string(string, '<');
            }
            input = getchar();
            if(isValidOper(input)){
                tokenToMake->value = string;
                return tokenToMake;
            }
            exit(1);
        case '>':
            if(nextChar == '=') {
                tokenToMake->tokenType = greaterEqual;
                add_str_to_string(string, ">=");
            }
            else{
                tokenToMake->tokenType = greater;
                add_char_to_string(string, '>');
            }
            input = getchar();
            if(isValidOper(input)){
                tokenToMake->value = string;
                return tokenToMake;
            }
            exit(1);
        case '=':
            if(nextChar == '=') {
                nextChar = getchar();
                if (nextChar == '=') {
                    tokenToMake->tokenType = cmpEqual;
                    add_str_to_string(string, "===");
                    nextChar = getchar();
                } else {
                    exit(1);
                }
            }
            else{
                tokenToMake->tokenType = equal;
                add_char_to_string(string, '=');
            }
            if(!isValidOper(nextChar)){
                exit(1);
            }
            ungetc(nextChar, stdin);
            tokenToMake->value = string;
            return tokenToMake;
        case '!':
            if(!isEqual(nextChar)){
                exit(1);
            }
            nextChar = getchar();
            if(!isEqual(nextChar)){
                exit(1);
            }
            tokenToMake->tokenType = notEquals;
            add_str_to_string(string, "!==");
            tokenToMake->value = string;
            ungetc(nextChar, stdin);
            return tokenToMake;
        case '{':
            tokenToMake->tokenType = openCurly;
            add_char_to_string(string, '{');
            break;
        case '}':
            tokenToMake->tokenType = closeCurly;
            add_char_to_string(string, '}');
            break;
        case '[':
            tokenToMake->tokenType = openSquare;
            add_char_to_string(string, '[');
            break;
        case ']':
            tokenToMake->tokenType = closeSquare;
            add_char_to_string(string, ']');
            break;
        case '(':
            tokenToMake->tokenType = openBracket;
            add_char_to_string(string, '(');
            break;
        case ')':
            tokenToMake->tokenType = closeBracket;
            add_char_to_string(string, ')');
            break;
        case '+':
            tokenToMake->tokenType = add;
            add_char_to_string(string, '+');
            break;
        case '-':
            tokenToMake->tokenType = sub;
            add_char_to_string(string, '-');
            break;
        case '*':
            tokenToMake->tokenType = mul;
            add_char_to_string(string, '*');
            break;
        case '/':
            tokenToMake->tokenType = ddiv;
            add_char_to_string(string, '/');
            break;
        case ',':
            tokenToMake->tokenType = comma;
            add_char_to_string(string, ',');
            break;
        case ';':
            tokenToMake->tokenType = semicolumn;
            add_char_to_string(string, ';');
            break;
    }
    if(isValidOper(nextChar)){
        tokenToMake->value = string;
        ungetc(nextChar, stdin);
        return tokenToMake;
    }
    exit(1);
}
