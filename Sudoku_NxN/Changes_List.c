#include "Changes_List.h"

/*createNewList allocates a new List instance and updates its field to default. It return the list pointer*/

List* createNewList(){
	List* list = (List*) calloc(1, sizeof(List));
	list->head=NULL;
	list->current = NULL;
	return list;
}

/*isListEmpty - returns true iff the list is empty (if the head is NULL) */

bool isListEmpty(List* list_p){
	return(list_p->head==NULL);
}

/*createNodeAfterCurrent -
 * pre:arr_length > 0 is the size of the (arr_length X arr_length) matrix "changes_matrix".
 * createNodeAfterCurrent creates a new node, representing a change which its type is change_type, and
 * the matrix which compress it is changes_matrix. The function sets the new change (Node) as the new current pointer
 * of the list and deletes all Nodes that were "after" the previous current using deleteAndFreeCellsToEnd.*/

void createNodeAfterCurrent(List* list_p, Change_Type change_type, int* changes_matrix, int arr_length){

	Node* node_p = (Node*) calloc(1, sizeof(Node));
	node_p->next = NULL;
	node_p->prev = list_p->current;
	node_p->change_type = change_type;
	node_p->changes_arr = changes_matrix;
	node_p->arr_len = arr_length; /*the arr is 2-d and its all size is arr_length*4*/
	if(isListEmpty(list_p)){
		list_p->head = node_p;
	}
	else{
		deleteAndFreeCellsToEnd(list_p->current->next);
		list_p->current->next = node_p;
	}
	list_p->current = node_p;
}

/*emptyList - gets a list pointer, and deletes every member of this list*/

void emptyList(List* list_p){
	if(!isListEmpty(list_p)){
		deleteAndFreeCellsToEnd(list_p->head);
		list_p->head=NULL;
		list_p->current=NULL;
	}
}

/*deleteAndFreeCellsToEnd -
 * pre: the head and the current cell of the list don't need to change in this function, meaning node_p is placed "after" them both.
 * This recursive function gets a node and deletes every next cell (including start_node_p) until it reaches NULL*/

void deleteAndFreeCellsToEnd(Node* start_node_p){
	Node* node_p = start_node_p;
	Node* next_node_p;

	if(node_p != NULL){
		next_node_p = node_p->next;
		node_p->prev = NULL;
		node_p->next = NULL;
		if(node_p->change_type == SET_CHANGE) /*if dynamic array was allocated*/{
			free(node_p->changes_arr);
		}
		free(node_p);
		deleteAndFreeCellsToEnd(next_node_p);
	}
}

/*setCurrentPrev
 * pre : the "current node" of the list is not NULL, and therefore has "previous" (that of course can be NULL). Since undo isn't available
 * in Init mode, and that is the only time that the current node can be NULL, the condition always exists.
 * The function tries to update the current pointer of the list to its "prev". If the prev is not NULL,
 * the update is made and the function returns true. Else, it returns false.*/

bool setCurrentPrev(List* list){
	Node* old_cur = list->current;
	if(old_cur->prev == NULL){
		return false;
	}
	else{
		list->current = old_cur->prev;
		return true;
	}
}

/*setCurrentNext tries to update the current pointer of the list to its "next". If the next is not NULL,
 * the update is made and the function returns true. Else, it returns false.*/

bool setCurrentNext(List* list){
	Node* old_cur = list->current;
	if(old_cur->next == NULL){
		return false;
	}
	else{
		list->current = old_cur->next;
		return true;
	}
}
