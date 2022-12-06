#include "DLTokenList.h"

token* DLTokenL_FetchNext(DLTokenL *list){
	DLTokenL_InsertLast(list, GetToken());
    //printf("Token: type = %s, value = %s\n", tokenTyp[DLTokenL_GetLast(list)->tokenType],DLTokenL_GetLast(list)->value->string);
	return DLTokenL_GetLast(list);
}

void DLTokenL_UnFetchNext(DLTokenL *list){
	UnGetToken(DLTokenL_GetLast(list));
	DLTokenL_DeleteLast(list);
}

DLTokenL* DLTokenL_CopyFromActive(DLTokenL *copiedList){
	if (!DLTokenL_IsActive)
	{
		DLTokenL_Dispose(copiedList); exit(99);
	}
	
	DLTokenL* listCopy = DLTokenL_Create();
	DLTokenLElementPtr keepActive = copiedList->activeElement;
	while (DLTokenL_IsActive(copiedList))
	{
		DLTokenL_InsertLast(listCopy, DLTokenL_GetActive(copiedList));
		DLTokenL_Next(copiedList);
	}
	copiedList->activeElement = keepActive;

	return listCopy;
}

DLTokenL* DLTokenL_Create(){
	DLTokenL* list = (DLTokenL*)malloc(sizeof(DLTokenL));
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
	return list;
}

void DLTokenLElement_Dispose(struct DLTokenLElement* element){
	free_token(element->token);
	free(element);
}

void DLTokenL_Dispose(DLTokenL *list) {
	DLTokenLElementPtr currElement = list->firstElement;
	DLTokenLElementPtr prevElement;
	while (currElement)
	{
		prevElement = currElement;
		currElement = currElement->nextElement;

		DLTokenLElement_Dispose(prevElement);
	}

	free(list);
}

void DLTokenL_Clear(DLTokenL *list) {
	DLTokenLElementPtr currElement = list->firstElement;
	DLTokenLElementPtr prevElement;
	while (currElement)
	{
		prevElement = currElement;
		currElement = currElement->nextElement;

		DLTokenLElement_Dispose(prevElement);
	}

	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

void DLTokenL_InsertFirst(DLTokenL *list, token* token) {
	DLTokenLElementPtr newHead = malloc(sizeof(struct DLTokenLElement));
	if (newHead == NULL)
	{
		exit(99);
	}

	newHead->token = token;
	newHead->previousElement = NULL;
	newHead->nextElement = list->firstElement;

	if(list->firstElement != NULL){
		list->firstElement->previousElement = newHead;
	}

	list->firstElement = newHead;
	if (list->lastElement == NULL)
	{
		list->lastElement = newHead;
	}
}

void DLTokenL_InsertLast(DLTokenL *list, token* token) {
	DLTokenLElementPtr newTail = malloc(sizeof(struct DLTokenLElement));
	if (newTail == NULL)
	{
		exit(99);
	}

	newTail->token = token;
	newTail->nextElement = NULL;
	newTail->previousElement = list->lastElement;

	if(list->lastElement != NULL){
		list->lastElement->nextElement = newTail;
	}

	list->lastElement = newTail;
	if (list->firstElement == NULL)
	{
		list->firstElement = newTail;
	}
}

void DLTokenL_First(DLTokenL *list) {
	list->activeElement = list->firstElement;
}

void DLTokenL_Last(DLTokenL *list) {
	list->activeElement = list->lastElement;
}

token* DLTokenL_GetFirst(DLTokenL *list) {
	if (list->firstElement == NULL)
	{
		exit(99);
	}

	return list->firstElement->token;
}

token* DLTokenL_GetLast(DLTokenL *list) {
	if (list->lastElement == NULL)
	{
		exit(99);
	}

	return list->lastElement->token;
}

struct DLTokenLElement* DLTokenL_GetLastElement(DLTokenL *list) {
	if (list->lastElement == NULL)
		exit(99);

	return list->lastElement;
}

void DLTokenL_DeleteFirst(DLTokenL *list) {
	if (list->firstElement == NULL)
	{
		return;
	} 
	
	if (list->firstElement == list->activeElement)
	{
		list->activeElement = NULL;
	}

	if (list->firstElement == list->lastElement)
	{
		list->lastElement = NULL;
	}
	

	DLTokenLElementPtr newHead = list->firstElement->nextElement;
	if (newHead != NULL)
	{
		newHead->previousElement = NULL;
	}
	
	free(list->firstElement);
	list->firstElement = newHead;
}

void DLTokenL_DeleteLast(DLTokenL *list) {
	if (list->lastElement == NULL)
		return;
	
	if (list->lastElement == list->activeElement)
		list->activeElement = NULL;

	if (list->firstElement == list->lastElement)
		list->firstElement = NULL;

	DLTokenLElementPtr newTail = list->lastElement->previousElement;
	if (newTail != NULL)
		newTail->nextElement = NULL;
	
	free(list->lastElement);
	list->lastElement = newTail;
}

void DLTokenL_DeleteAfter(DLTokenL *list) {
	if (list->activeElement == NULL || list->activeElement->nextElement == NULL)
	{
		return;
	}

	DLTokenLElementPtr deletedElement = list->activeElement->nextElement;

	if (deletedElement == list->lastElement)
	{
		list->lastElement = deletedElement->previousElement;
	}
	
	list->activeElement->nextElement = deletedElement->nextElement;
	if (deletedElement->nextElement != NULL)
	{
		deletedElement->nextElement->previousElement = list->activeElement;
	}
	
	free(deletedElement);
}

void DLTokenL_DeleteBefore(DLTokenL *list) {
	if (list->activeElement == NULL || list->activeElement->previousElement == NULL)
	{
		return;
	}

	DLTokenLElementPtr deletedElement = list->activeElement->previousElement;

	if (deletedElement == list->firstElement)
	{
		list->firstElement = deletedElement->nextElement;
	}
	
	list->activeElement->previousElement = deletedElement->previousElement;
	if (deletedElement->previousElement != NULL)
	{
		deletedElement->previousElement->nextElement = list->activeElement;
	}
	
	free(deletedElement);
}

void DLTokenL_InsertAfter(DLTokenL *list, token* token) {
	if (list->activeElement == NULL)
	{
		return;
	}

	DLTokenLElementPtr newElement = malloc(sizeof(struct DLTokenLElement));
	if (newElement == NULL)
	{
		exit(99);
	}
	newElement->token = token;

	DLTokenLElementPtr movedElement = list->activeElement->nextElement;
	newElement->nextElement = movedElement;
	if (movedElement != NULL)
	{
		movedElement->previousElement = newElement;
	}

	newElement->previousElement = list->activeElement;
	list->activeElement->nextElement = newElement;
}

void DLTokenL_InsertBefore(DLTokenL *list, token* token) {
if (list->activeElement == NULL)
	{
		return;
	}

	DLTokenLElementPtr newElement = malloc(sizeof(struct DLTokenLElement));
	if (newElement == NULL)
	{
		exit(99);
	}
	newElement->token = token;

	DLTokenLElementPtr movedElement = list->activeElement->previousElement;
	newElement->previousElement = movedElement;
	if (movedElement != NULL)
	{
		movedElement->nextElement = newElement;
	}

	newElement->nextElement = list->activeElement;
	list->activeElement->previousElement = newElement;
}

void DLTokenL_Next(DLTokenL *list) {
	if (list->activeElement == NULL)
	{
		return;
	}
	list->activeElement = list->activeElement->nextElement;
}

void DLTokenL_Previous(DLTokenL *list) {
	if (list->activeElement == NULL)
	{
		return;
	}
	list->activeElement = list->activeElement->previousElement;
}

bool DLTokenL_IsActive(DLTokenL *list) {
	return list->activeElement != NULL;
}

token* DLTokenL_GetActive(DLTokenL *list) {
	if (!DLTokenL_IsActive(list))
		return NULL;

	return list->activeElement->token;
}