#include "scanner.h"

char input;

token* GetToken() {
    input = getchar();
    if (isQuestion(input)) {
        return Type();
    }
    if (isLetterUnder(input)) {
        return Word();
    }
    if (isDollar(input)) {
        return Variable();
    }
    if (isdigit(input)) {
        return Number();
    }
    if (isQuot(input)) {
        return String();
    }
    if (isOperator(input)) {
        return Operator();
    }
}

token* Prolog(token* tok) {
    char tmp1 = getchar();
    char tmp2 = getchar();
    char tmp3 = getchar();
    if (tmp1 != 'p' || tmp2 != 'h' || tmp3 != 'p') {
        exit(2);
    }
    input = getchar();
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
    tok->tokenType = prolog;
    return tok;
}

token* Declare(token* tok){
    input = getchar();
    if(!isOpenBracket(input)){
        exit(1);
    }
    free_string(tok->value);
    initialize_string(tok->value);
    input = getchar();
    while(!isCloseBracket(input)){
        add_char_to_string(tok->value,input);
        input = getchar();
    }
    tok->tokenType = declare;
    return tok;
}

token* makeToken(){
    printf("makeToken\n");
    dynamic_string string;
    initialize_string(&string);
    token * tokenToMake = (token*)malloc(sizeof (token));
    if(!tokenToMake){
        exit(99);
    }
    tokenToMake->value = &string;
    printf("madeToken\n");
    return tokenToMake;
}

token* Type(){
    token* tokenToMake = makeToken();
    tokenToMake->tokenType = type;
    add_char_to_string(tokenToMake->value, input);
    input = getchar();
    if(input == '>'){
        tokenToMake->tokenType = end;
        return tokenToMake;
    }
    while(isalpha(input)){
        add_char_to_string(tokenToMake->value, input);
        input = getchar();
    }
    if(!isType(tokenToMake->value)){
        exit(1);
    }
    if(!isValidText(input)){
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
    if(strcmp(tokenToMake->value->string, "declare") == 0){
        return Declare(tokenToMake);
    }
    if(isType(tokenToMake->value)){
        tokenToMake->tokenType = type;
    }
    if(isKeyword(tokenToMake->value)){
        tokenToMake->tokenType = keyword;
    }
    if(!isValidText(input)){
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
    if(!isValidText(input)){
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
    if(!isValidText(input)){
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
    if(!isValidText(input)){
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
    if(!isValidText(input)){
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
    while(!isQuot(input) || isBackslash(prevChar)){
        //TODO escape sekvence
        if(isBackslash(input)){

        }
        add_char_to_string(tokenToMake->value, input);
    }
    input = getchar();
    if(isValidText(input)){
        return tokenToMake;
    }
    exit(1);
}

token* Operator(){
    token *tokenToMake = makeToken();
    char nextChar = getchar();
    switch (input) {
        case '<':
            if(nextChar == '?'){
                return Prolog(tokenToMake);
            }
            if(nextChar == '=') {
                tokenToMake->tokenType = lowerEqual;
                add_str_to_string(tokenToMake->value, "<=");
            }
            else{
                tokenToMake->tokenType = lower;
                add_char_to_string(tokenToMake->value, '<');
            }
            input = getchar();
            if(isValidOper(input)){
                return tokenToMake;
            }
            exit(1);
        case '>':
            if(nextChar == '=') {
                tokenToMake->tokenType = greaterEqual;
                add_str_to_string(tokenToMake->value, ">=");
            }
            else{
                tokenToMake->tokenType = greater;
                add_char_to_string(tokenToMake->value, '>');
            }
            input = getchar();
            if(isValidOper(input)){
                return tokenToMake;
            }
            exit(1);
        case '=':
            if(nextChar == '=') {
                nextChar = getchar();
                if (nextChar == '=') {
                    tokenToMake->tokenType = cmpEqual;
                    add_str_to_string(tokenToMake->value, "===");
                } else {
                    exit(1);
                }
            }
            else{
                if(isValidOper(nextChar)){
                    tokenToMake->tokenType = equal;
                    add_char_to_string(tokenToMake->value, '=');
                    ungetc(nextChar, stdin);
                } else{
                    exit(1);
                }
            }
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
            add_str_to_string(tokenToMake->value, "!==");
            return tokenToMake;
        case '{':
            tokenToMake->tokenType = openCurly;
            add_char_to_string(tokenToMake->value, '{');
            break;
        case '}':
            tokenToMake->tokenType = closeCurly;
            add_char_to_string(tokenToMake->value, '}');
            break;
        case '[':
            tokenToMake->tokenType = openSquare;
            add_char_to_string(tokenToMake->value, '[');
            break;
        case ']':
            tokenToMake->tokenType = closeSquare;
            add_char_to_string(tokenToMake->value, ']');
            break;
        case '(':
            tokenToMake->tokenType = openBracket;
            add_char_to_string(tokenToMake->value, '(');
            break;
        case ')':
            tokenToMake->tokenType = closeBracket;
            add_char_to_string(tokenToMake->value, ')');
            break;
        case '+':
            tokenToMake->tokenType = add;
            add_char_to_string(tokenToMake->value, '+');
            break;
        case '-':
            tokenToMake->tokenType = sub;
            add_char_to_string(tokenToMake->value, '-');
            break;
        case '*':
            tokenToMake->tokenType = mul;
            add_char_to_string(tokenToMake->value, '*');
            break;
        case '/':
            tokenToMake->tokenType = ddiv;
            add_char_to_string(tokenToMake->value, '/');
            break;
        case ',':
            tokenToMake->tokenType = comma;
            add_char_to_string(tokenToMake->value, ',');
            break;
        case ';':
            tokenToMake->tokenType = semicolumn;
            add_char_to_string(tokenToMake->value, ';');
            break;
    }
    if(isValidOper(nextChar)){
        return tokenToMake;
    }
    exit(1);
}
