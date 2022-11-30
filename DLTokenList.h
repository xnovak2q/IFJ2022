#ifndef IFJ2022_DLTOKENLIST_H
#define IFJ2022_DLTOKENLIST_H

#include <stdlib.h>
#include "scanner.h"

/** Prvek dvousměrně vázaného seznamu. */
typedef struct DLTokenLElement {
	token* token;
	/** Ukazatel na předcházející prvek seznamu. */
	struct DLTokenLElement *previousElement;
	/** Ukazatel na následující prvek seznamu. */
	struct DLTokenLElement *nextElement;
} *DLTokenLElementPtr;

/** Dvousměrně vázaný seznam. */
typedef struct {
	/** Ukazatel na první prvek seznamu. */
	DLTokenLElementPtr firstElement;
	/** Ukazatel na aktuální prvek seznamu. */
	DLTokenLElementPtr activeElement;
	/** Ukazatel na posledni prvek seznamu. */
	DLTokenLElementPtr lastElement;
} DLTokenL;

token* DLTokenL_FetchNext(DLTokenL *list);
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

#endif //IFJ2022_DLTOKENLIST_H