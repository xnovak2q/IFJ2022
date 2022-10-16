//
// Created by Lakluk on 15.10.2022.
//

#ifndef IFJ2022_BASICSCANNERFUNCTIONS_H
#define IFJ2022_BASICSCANNERFUNCTIONS_H

#include "string.h"
#include "ctype.h"
#include "stdio.h"
#include "dynamic_string.h"

bool isLetterUnder(char x);
bool isLetterUnderNumber(char x);
bool isKeyword(dynamic_string *x);
bool isQuot(char x);
bool isBackSlash(char x);
bool isDot(char x);
bool isE(char x);
bool isMinus(char x);
bool isOpenBracket(char x);
bool isCloseBracket(char x);
bool isMinusPlus(char x);
bool isOperator(char x);
bool isEqual(char x);
bool isSlash(char x);
bool isEOF(char x);
void skipLineComment();
void skipBlockComment();
bool isLine(char x);
bool isStar(char x);
bool isQuestion(char x);
bool isLetter(char x);
bool isType(dynamic_string *x);
bool isDollar(char x);
bool isValid(char x);

#endif //IFJ2022_BASICSCANNERFUNCTIONS_H
