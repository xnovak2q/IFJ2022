
/* ******************************* c202.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/*          Daniel Dolejška, září 2021                                        */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znaků ve statickém poli. ADT zásobník je
** reprezentován záznamem typu Stack. Statické pole 'array' je indexováno
** do maximální hodnoty STACK_SIZE. Položka záznamu 'topIndex' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota topIndex == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Přesnou definici typů neleznete
** v hlavičkovém souboru c202.h.
**
** Implementujte následující funkce:
**
**    Stack_Init .... inicializace zásobníku
**    Stack_IsEmpty ... test na prázdnost zásobníku
**    Stack_IsFull .... test na zaplněnost zásobníku
**    Stack_Top ..... přečte hodnotu z vrcholu zásobníku
**    Stack_Pop ..... odstraní prvek z vrcholu zásobníku
**    Stack_Push .... vloží prvku do zásobníku
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "postfixStack.h"

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 * Hodnoty ve statickém poli neupravujte - po inicializaci zásobníku
 * jsou nedefinované.
 *
 * V případě, že funkce dostane jako parametr stack == NULL,
 * volejte funkci Stack_Error(SERR_INIT).
 * U ostatních funkcí pro zjednodušení předpokládejte, že tato situace
 * nenastane.
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void Stack_Init(postStack_t *stack)
{
    if (stack == NULL)
    {
        return;
    };
    stack->topIndex = -1; // pokud je stack prázdný, jeho topIndex by měl mít hodnotu -1
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník prázdný, jinak vrací hodnotu 0.
 * Funkci implementujte jako jediný příkaz.
 * Vyvarujte se zejména konstrukce typu "if ( cond ) b=true else b=false".
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník prázdný, jinak nulu
 */
int Stack_IsEmpty(const postStack_t *stack)
{
    return stack->topIndex == -1; // pokud je prázdný podmínka je splněna (topIndex == -1), tak vrací 1, jinak 0
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 * Dejte si zde pozor na častou programátorskou chybu "o jedničku" a dobře se
 * zamyslete, kdy je zásobník plný, jestliže může obsahovat nejvýše STACK_SIZE
 * prkvů a první prvek je vložen na pozici 0.
 *
 * Funkci implementujte jako jediný příkaz.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull(const postStack_t *stack)
{
    return stack->topIndex == MAX_STACK - 1; // pokud je prázdný podmínka je splněna (topIndex == STACK_SIZE), tak vrací 1, jinak 0
}

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 * Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
 * Volání operace Top při prázdném zásobníku je nekorektní a ošetřete ho voláním
 * procedury Stack_Error(SERR_TOP).
 *
 * Pro ověření, zda je zásobník prázdný, použijte dříve definovanou funkci
 * Stack_IsEmpty.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Stack_Top(const postStack_t *stack, char *dataPtr)
{
    if (Stack_IsEmpty(stack))
    {
        return;
    }
    *dataPtr = stack->array[stack->topIndex]; // podívám se na topIndex stacku a jeho adresu vložím do ukazatele
}

/**
 * Odstraní prvek z vrcholu zásobníku. Pro ověření, zda je zásobník prázdný,
 * použijte dříve definovanou funkci Stack_IsEmpty.
 *
 * Vyvolání operace Pop při prázdném zásobníku je sice podezřelé a může
 * signalizovat chybu v algoritmu, ve kterém je zásobník použit, ale funkci
 * pro ošetření chyby zde nevolejte (můžeme zásobník ponechat prázdný).
 * Spíše než volání chyby by se zde hodilo vypsat varování, což však pro
 * jednoduchost neděláme.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void Stack_Pop(postStack_t *stack)
{
    if (!Stack_IsEmpty(stack))
    {
        stack->topIndex--; // tím, že snížím topIndex o 1, tak zahodím adresu vrchního prvku
    }
}

/**
 * Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
 * je nekorektní a ošetřete ho voláním procedury Stack_Error(SERR_PUSH).
 *
 * Pro ověření, zda je zásobník plný, použijte dříve definovanou
 * funkci Stack_IsFull.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void Stack_Push(postStack_t *stack, char data)
{
    if (Stack_IsFull(stack))
    {
        return;
    }
    stack->topIndex++;                    // napřed musím zvýšit velikost stacku
    stack->array[stack->topIndex] = data; // do prázdné buňky vložím data
}

/* Konec postfixStack.c */
