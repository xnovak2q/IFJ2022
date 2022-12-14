//
// Autor: xfuisl00
// Pomocné funkce pro scanner
//

#include "basicScannerFunctions.h"

/*
 * Pole obsahující hodnoty klíčových slov
*/
const char *keywords[] = {
        "else", "function", "if", "null", "return", "while"
};

/*
 * Pole obsahující hodnoty typů
*/
const char *types[] = {
        "float", "int", "string", "void"
};

/*
 * Pole obsahující hodnoty nullovatelných typů
*/
const char *nullableTypes[] = {
        "?float", "?int", "?string", "?void"
};

bool isLetterUnder(char x){
    return isalpha(x) || x == '_';
}

bool isLetterUnderNumber(char x){
    return isLetterUnder(x) || isdigit(x);
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
    return x == '(';
}

bool isCloseBracket(char x){
    return x == ')';
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
        case ';':
        case '!':
        case ':':
        case '.':
            return true;
        default:
            return isMinusPlus(x) || isEqual(x) || isSlash(x) || isQuestion(x);
    }
}
bool isEqual(char x){
    return x == '=';
}
bool isSlash(char x){
    return x == '/';
}
bool isEOF(char x){
    return x == EOF || x == '\0';
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

int isType(dynamic_string x){
    for(int i = 0; i < 4; i++){
        if(!strcmp(x.string, types[i]))
            return i;
    }
    return -1;
}

int isNullableType(dynamic_string x){
    for(int i = 0; i < 4; i++){
        if(!strcmp(x.string, nullableTypes[i]))
            return i;
    }
    return -1;
}

int isKeyword(dynamic_string x){
    for(int i = 0; i < 6; i++){
        if(!strcmp(x.string, keywords[i]))
            return i;
    }
    return -1;
}

bool isDollar(char x){
    return x == '$';
}

/*
 * Funkce vracející true pokud znak v parametru je validní jako další znak po neoperátorovém tokenu.
 * Po neoperátorovém tokenu je validní znak operátor, mezera nebo EOF
*/
bool isValidText(char x){
    return isOperator(x) || isspace(x) || isEOF(x);
}

/*
 * Funkce vracející true pokud znak v parametru je validní jako další znak po operátorovém tokenu.
 * Po operátorovém tokenu je validní znak znak, číslo, undescore, dollar, mezera, uvozovky nebo EOF
*/
bool isValidOper(char x){
    return isLetterUnderNumber(x) || isDollar(x) || isspace(x) || isOperator(x) || isQuot(x) || isEOF(x);
}