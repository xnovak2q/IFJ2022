#include "scanner.h"

/*
 * Lexikální analýza
*/
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
    if(isspace(input)){
        return GetToken();
    }
    if(isEOF(input)){
        dynamic_string *string = malloc(sizeof(dynamic_string));
        initialize_string(string);
        return makeToken(string, end);
    }
    exit(1);
}

/*
 * Funkce zpracovávající vstup na token prolog
*/
token* Prolog() {
    //Načtení 3 znaků, které mají obsahovat "php"
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

/*
 * Funkce zpracovávající vstup na token declare
*/
token* Declare(dynamic_string* string){
    char input = getchar();
    if(!isOpenBracket(input)){
        exit(1);
    }
    free_string(string);
    initialize_string(string);
    input = getchar();
    //Načítání obsahu závorek
    while(!isCloseBracket(input)){
        add_char_to_string(string,input);
        input = getchar();
    }
    input = getchar();
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string,declare);
}

/*
 * Funkce zpracovávající vstup na token type
*/
token* Type(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    // Pokud na vstupu je ">" není výsledný token type, nýbrž se jedná o konec programu
    if(input == '>'){ //
        return makeToken(string, end);
    }
    while(isalpha(input)){
        add_char_to_string(string, input);
        input = getchar();
    }

    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);

    //Switche pro získání správného typu tokenu
    switch(isType(*string)){
        case 0: return makeToken(string, typeFloat);
        case 1: return makeToken(string, typeInt);
        case 2: return makeToken(string, typeString);
        case 3: return makeToken(string, typeVoid);
        default: break;
    }

    switch(isNullableType(*string)){
        case 0: return makeToken(string, nullableFloat);
        case 1: return makeToken(string, nullableInt);
        case 2: return makeToken(string, nullableString);
        case 3: return makeToken(string, nullableVoid);
        default: break;
    }

    exit(1);
}

/*
 * Funkce zpracovávající vstup na token keyword, type nebo identificator
*/
token* Word(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    while(isLetterUnder(input) || isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    if(strcmp(string->string, "declare") == 0){
        ungetc(input, stdin);
        return Declare(string);
    }

    if(!isValidText(input)){
        exit(1);
    }

    ungetc(input, stdin);
    switch(isKeyword(*string)){
        case 0: return makeToken(string, eelse);
        case 1: return makeToken(string, ffunction);
        case 2: return makeToken(string, iff);
        case 3: return makeToken(string, nnull);
        case 4: return makeToken(string, rreturn);
        case 5: return makeToken(string, wwhile);
        default: break;
    }

    switch(isType(*string)){
        case 0: return makeToken(string, typeFloat);
        case 1: return makeToken(string, typeInt);
        case 2: return makeToken(string, typeString);
        case 3: return makeToken(string, typeVoid);
        default: break;
    }

    return makeToken(string, identificator);
}

/*
 * Funkce zpracovávající vstup na token variable
*/
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

/*
 * Funkce zpracovávající vstup na token integer, ffloat nebo exponent
*/
token* Number(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    add_char_to_string(string, input);
    input = getchar();
    while(isdigit(input)){
        add_char_to_string(string, input);
        input = getchar();
    }
    //Pokud přijde na vstup "." pak se jedná o token typu float
    if(isDot(input)){
        add_char_to_string(string, input);
        return Float(string,input);
    }
    //Pokud přijde na vstup "e" nebo "E" pak se jedná o token typu exponent
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

/*
 * Funkce zpracovávající vstup na token exponent
*/
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

/*
 * Funkce zpracovávající vstup na token ffloat
*/
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

/*
 * Funkce zpracovávající vstup na token sstring
*/
token* String(){
    char input = getchar();
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    char prevChar;
    //Cyklus while končí když na vstupu je "\"" a zároveň předchozí znak není "\"
    while(!isQuot(input) || isBackslash(prevChar)){
        add_char_to_string(string, input);
        input = getchar();
    }
    input = getchar();
    if(!isValidText(input)){
        exit(1);
    }
    ungetc(input, stdin);
    return makeToken(string, sstring);
}

/*
 * Funkce zpracovávající vstup na operátorové tokeny
*/
token* Operator(char input){
    dynamic_string* string = malloc(sizeof(dynamic_string));
    initialize_string(string);
    token *tokenToMake = makeToken(string, opperator);
    token *temp;
    char nextChar = getchar();
    switch (input) {
        case '<':
            if(nextChar == '?'){
                free_token(tokenToMake);
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
                    add_str_to_string(string, "===");
                    temp = makeToken(string,cmpEqual);
                    nextChar = getchar();
                } else {
                    exit(1);
                }
            }
            else{
                add_char_to_string(string, '=');
                temp = makeToken(string, equal);
            }
            if(!isValidOper(nextChar)){
                exit(1);
            }
            ungetc(nextChar, stdin);
            return temp;
        case '!':
            if(!isEqual(nextChar)){
                exit(1);
            }
            nextChar = getchar();
            if(!isEqual(nextChar)){
                exit(1);
            }
            nextChar = getchar();
            if(!isValidOper(nextChar)){
                exit(1);
            }
            add_str_to_string(string, "!==");
            temp = makeToken(string, notEquals);
            ungetc(nextChar, stdin);
            return temp;
        case ':':
            tokenToMake->tokenType = colon;
            add_char_to_string(string, ':');
            break;
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
            if(nextChar == '/'){
                skipLineComment();
                return GetToken();
            }
            if(nextChar == '*'){
                skipBlockComment();
                return GetToken();
            }
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
        case '.':
            tokenToMake->tokenType = concat;
            add_char_to_string(string, '.');
            break;
    }
    if(isValidOper(nextChar)){
        token  * temp = makeToken(string, tokenToMake->tokenType);
        if (isEOF(nextChar)) ungetc('\0', stdin);
        else ungetc(nextChar, stdin);
        return temp;
    }
    exit(1);
}

/*
 * Funkce na vytvoření tokenu typu type s value string
*/
token* makeToken(dynamic_string* string, enum tokenType type){
    token * tokenToMake = malloc(sizeof (token));
    if(!tokenToMake){
        exit(99);
    }
    tokenToMake->value = string;
    tokenToMake->tokenType = type;
    return tokenToMake;
}

/*
 * Funkce na uvolnění paměti tokenu
*/
void free_token(token* token){
    if(token->tokenType == 30)
        return;
    free_string(token->value);
    free(token);
}

/*
 * Funkce vracející hodnotu tokenu zpět na stdin
*/
void UnGetToken(token* token){
    ungetc(' ', stdin);
    
    if (token->tokenType == sstring)
        ungetc('"', stdin);

    for(size_t i = token->value->string_length; i > 0; i--)
        ungetc(token->value->string[i-1], stdin);
        
    if (token->tokenType == variable)
        ungetc('$', stdin);
    else if (token->tokenType == sstring)
        ungetc('"', stdin);
    else if (token->tokenType == end)
        ungetc('\0', stdin);
    
    ungetc(' ', stdin);
    free_token(token);
}