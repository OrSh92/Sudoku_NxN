#ifndef CHANGES_LIST_H_
#define CHANGES_LIST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*The Changes_List module is responsible for the undo/redo doubly linked list structure, and its functions. The list
 * is a list of changes (while change means the difference between new board comparing to old board after one command
 * - meaning, in this module a "change" can contain many cell changes, for example after autofill). Every such change is
 * represented by Node.*/


/*The Enum Change_Type has two options - LOAD_CHANGE and SET_CHANGE, stating whether the change is from loading to the board
 * or setting(=filling) it*/

typedef enum {
	LOAD_CHANGE, SET_CHANGE
} Change_Type;

/*The structure Node represent a "change" (read more in the description above), and therefore:
 * - as a part of doubly linked list - contains its next and prev Node.
 * - as a rep of change - contains the change_type, and the changes_matrix that compress the change's full details, as well
 * as the length of this matrix. */

struct Node {
    struct Node* next;
    struct Node* prev;
    Change_Type change_type;
    int* changes_arr;
    int arr_len;
};

typedef struct Node Node;

/*The structure List contains two pointers - one to the head, and one to the current pointer*/

typedef struct
{
    Node* head;
    Node* current;
} List;

/*For simplicity, the explanations about the functions will be written in the source file*/

List* createNewList();
bool isListEmpty(List* list_p);
void createNodeAfterCurrent(List* list_p, Change_Type change_type, int* changes_matrix, int arr_length);
void emptyList(List* list_p);
void deleteAndFreeCellsToEnd(Node* start_node_p);
bool setCurrentPrev(List* list);
bool setCurrentNext(List* list);



#endif /* CHANGES_LIST_H_ */
