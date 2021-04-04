#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(struct list_type *l) {
	l->head = NULL;
	l->tail = NULL;
}
	
	
void insert_head(struct list_type *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
			l->tail = l->head;
}


void* extract_head(struct list_type *l) {
	/* Store the content of head; 
	   If l->head->next is not NULL, make the next cell head and free the original head; 
	   If l->head->next is NULL which means it is the free last one, free head. 
	   Return: the stored content of head */ 
	void* result; 
	if(l->head != NULL){ 
		result = l->head->content; 
		if (l->head->next != NULL) {
			l->head = l->head->next; 
			free(l->head->previous); 
			l->head->previous = NULL; 
		} 
		else { 
			free(l->head); 
		}
		return result; 
	} 
	return NULL; 
}


void* extract_tail(struct list_type *l) {
	/* Store the content of l->tail; 
	   If l->tail->previous is not NULL which means it is not the last cell, make the its previous cell the new tail and free the original tail; 
	   If l->tail->previous is NULL which means it is the last cell, free tail. 
	   Return: the stored content of tail */ 
	void* result; 
	if(l->tail != NULL){
		result = l->tail->content; 
		if (l->tail->previous != NULL){  // it's not head 
			l->tail = l->tail->previous; 
			free(l->tail->next); 
			l->tail->next = NULL; 
		} 
		else{ // it is head 
			free(l->tail); 
		}
		return result; 
	} 	
	return NULL;
} 


int list_size(struct list_type *l) {
	/* Use a counter, a temp *cell, and a while-loop */
	int count = 0; 
	cell* tempForCount = l->head; 

	while(tempForCount->next != NULL){
		count++;
		tempForCount = tempForCount->next; 
	}
	count++; 

	return count; 
}
	