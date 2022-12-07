/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */

#ifndef INFIX2POSTFIX_H
#define INFIX2POSTFIX_H

#include <stdbool.h>
#include <stdlib.h>
#include "postfixStack.h"
#include "DLTokenList.h"

typedef enum {
    plus_minus_dot_p,
    mul_div_p,
    rel_ops_p,
    eq_neq_p,
    l_bracket_p
} postTypes;

bool check_operand(token* token);
bool check_operator(token* token);
void untilLeftPar(postStack_t *stack, DLTokenL *postfixExpression);
void doOperation(postStack_t *stack, token* c, DLTokenL *postfixExpression);
DLTokenL* infix2postfix( DLTokenL *infixExpression );

#endif