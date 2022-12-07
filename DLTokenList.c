//
// Autor: xkonec86
//

#include "DLTokenList.h"

void DLTokenL_Print(DLTokenL *list){
	DLTokenLElementPtr currElement = list->firstElement;
	int maxTypeIndentation = 0;
	while (currElement)
	{
		if (maxTypeIndentation < currElement->token->value->string_length)
			maxTypeIndentation = currElement->token->value->string_length;
		currElement = currElement->nextElement;
	}

	currElement = list->firstElement;
	int i = 0;
	int currTypeIndentation;
	while (currElement)
	{
		printf("\x1B[92m%d: %s",i,currElement->token->value->string);
		currTypeIndentation = maxTypeIndentation - currElement->token->value->string_length;
		if (currTypeIndentation > 0)
			for(size_t indent_i = 0; indent_i < currTypeIndentation; indent_i++) printf(" ");
		
		//printf(" (%s)\033[0m\n",tokenTypos[currElement->token->tokenType]);
		currElement = currElement->nextElement; i++;
		printf("\033[0m\n");
	}
}

token* DLTokenL_FetchNext(DLTokenL *list){
	if (list->lastElement != NULL && DLTokenL_GetLast(list)->tokenType == end){
		DLTokenL_InsertLast(list, makeToken(NULL, end));
	} else {
		DLTokenL_InsertLast(list, GetToken());
	}

    //printf("Fetch token: %s (%s)\n",DLTokenL_GetLast(list)->value->string, tokenTypos[DLTokenL_GetLast(list)->tokenType]);
	return DLTokenL_GetLast(list);
}

void DLTokenL_UnFetchNext(DLTokenL *list){
	//printf("UnFetch token: %s (%s)\n",DLTokenL_GetLast(list)->value->string, tokenTypos[DLTokenL_GetLast(list)->tokenType]);
	UnGetToken(DLTokenL_GetLast(list));
	DLTokenL_DeleteLast(list);
}
void DLTokenL_UnFetchAll(DLTokenL *list){
	while (list->lastElement != NULL)
		DLTokenL_UnFetchNext(list);
}

DLTokenL* DLTokenL_CopyFromActive(DLTokenL *copiedList){
	if (!DLTokenL_IsActive(copiedList))
	{
		DLTokenL_Dispose(copiedList); exit(99);
	}
	
	DLTokenL* listCopy = DLTokenL_Create();
	while (DLTokenL_IsActive(copiedList))
	{
		dynamic_string* stringCopy = malloc(sizeof(dynamic_string));
		initialize_string(stringCopy);
		add_str_to_string(stringCopy, DLTokenL_GetActive(copiedList)->value->string);


		DLTokenL_InsertLast(listCopy, makeToken(stringCopy, DLTokenL_GetActive(copiedList)->tokenType));
		DLTokenL_Next(copiedList);
	}

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
	if (list->lastElement == NULL){
		exit(99);
	}

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

size_t DLTokenL_GetLength(DLTokenL *list){
	if (list == NULL) return 0;

	size_t length = 0;
	DLTokenLElementPtr currElement = list->firstElement;
	while (currElement)
	{
		length++;
		currElement = currElement->nextElement;
	}

	return length;
}

DLTokenLElementPtr DLTokenL_GetActiveElement(DLTokenL* list){
    if(!DLTokenL_IsActive(list))
        return NULL;
    return list->activeElement;
}