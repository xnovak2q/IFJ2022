//
// Autor: xkonec86
// Dvousměrně vázaný seznam tokenů
//

#ifndef IFJ2022_DLTOKENLIST_H
#define IFJ2022_DLTOKENLIST_H

#include <stdlib.h>
#include "scanner.h"

/** Uzel dvousměrně vázaného seznamu */
typedef struct DLTokenLElement {
	/** Ukazatel na token */
	token* token;
	/** Ukazatel na předcházející uzel seznamu */
	struct DLTokenLElement *previousElement;
	/** Ukazatel na následující uzel seznamu */
	struct DLTokenLElement *nextElement;
} *DLTokenLElementPtr;

/** Dvousměrně vázaný seznam */
typedef struct {
	/** Ukazatel na první uzel seznamu */
	DLTokenLElementPtr firstElement;
	/** Ukazatel na aktivní uzel seznamu */
	DLTokenLElementPtr activeElement;
	/** Ukazatel na posledni uzel seznamu */
	DLTokenLElementPtr lastElement;
} DLTokenL;

void DLTokenL_Print(DLTokenL *list);
token* DLTokenL_FetchNext(DLTokenL *list);
void DLTokenL_UnFetchNext(DLTokenL *list);
void DLTokenL_UnFetchAll(DLTokenL *list);
struct DLTokenLElement* DLTokenL_GetLastElement(DLTokenL *list);
DLTokenL* DLTokenL_Create();
void DLTokenL_Dispose(DLTokenL*);
void DLTokenL_Clear(DLTokenL*);
void DLTokenL_InsertFirst(DLTokenL*, token*);
void DLTokenL_InsertLast(DLTokenL*, token*);
void DLTokenL_First(DLTokenL*);
void DLTokenL_Last(DLTokenL*);
token* DLTokenL_GetFirst(DLTokenL*);
token* DLTokenL_GetLast(DLTokenL*);
void DLTokenLElement_Dispose(struct DLTokenLElement* element);
void DLTokenL_DeleteFirst(DLTokenL*);
void DLTokenL_DeleteLast(DLTokenL*);
void DLTokenL_DeleteAfter(DLTokenL*);
void DLTokenL_DeleteBefore(DLTokenL*);
void DLTokenL_InsertAfter(DLTokenL*, token*);
void DLTokenL_InsertBefore(DLTokenL*, token*);
void DLTokenL_Next(DLTokenL*);
void DLTokenL_Previous(DLTokenL*);
bool DLTokenL_IsActive(DLTokenL*);
token* DLTokenL_GetActive(DLTokenL *list);
DLTokenL* DLTokenL_CopyFromActive(DLTokenL *copiedList);
size_t DLTokenL_GetLength(DLTokenL *list);
DLTokenLElementPtr DLTokenL_GetActiveElement(DLTokenL* list);

#endif //IFJ2022_DLTOKENLIST_H