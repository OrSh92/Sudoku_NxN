#include "Changes_Manager.h"

/*saveAndMakeChanges -
 * pre: the new_matrix is the old one after a single user's command, not including Solve or Edit - which doesn't only change the board
 * but changes the game and recreate the changes list.
 *  This function gets an old matrix and a new matrix and checks if there is a change between them. If there is,
 * it compresses the changes using compressChangesTo and add the change to the changes list using newLastMoveSet. At the end
 * it reflects the changes in the board . to_print decides whether the changes are printed or not.
 * The function returns true iff there was a change*/

bool saveAndMakeChanges(Game* game_pointer, int* old_matrix, int* new_matrix, bool to_print){
	int* changes_matrix;
	int num_of_changes = numOfChanges(old_matrix, new_matrix, game_pointer->blocks);
	if(num_of_changes==0){
		printf("No changes were made\n");
		return false;
	}

	changes_matrix = (int *)malloc(4 * num_of_changes * sizeof(int));
	if(changes_matrix==NULL)		{
		printf("Error: an allocation error has occurred, exiting...\n");
		exit(1);
	}
	compressChangesTo(changes_matrix, old_matrix, new_matrix, game_pointer->blocks);
	newLastMoveSet(game_pointer, changes_matrix, num_of_changes);
	makeChanges(game_pointer, changes_matrix, num_of_changes, false, to_print);
	return true;
}

/*makeChanges
 * pre: at least one change was made (num_of_changes>0).
 * The function decompresses the changes_matrix, and changes the board accordingly. The bool back is used to know
 * the direction of the change (changes due to redo and undo are opposite). The second bool to_print is used to know whether
 * the changes need to be printed or not. The function returns nothing*/

void makeChanges(Game* game_pointer, int* changes_matrix, int num_of_changes, bool back, bool to_print){
	int old_index = 2, new_index = 3, i;
	if(back){
		old_index = 3, new_index = 2;
	}
	if(to_print){
	printf("The following changes were made:\n");
	}
	for(i=0; i<num_of_changes; i++){
		game_pointer->current_puzzle[(changes_matrix[4*i+1]-1)*game_pointer->blocks + (changes_matrix[4*i]-1)] =
				changes_matrix[4*i+new_index];
		if(to_print){
		printf("In (col=%d, row=%d) %d was changed to %d\n", changes_matrix[4*i], changes_matrix[4*i+1],
				changes_matrix[4*i+old_index],changes_matrix[4*i+new_index]);
		}
	}
	if(to_print){
		printf("The current board is:\n");
		printBoard(game_pointer);
	}
}

/*newLastMoveSet adds a "Set Change", which contains the changes_matrix, to the changes list.
 * This is done with the help of createNodeAfterCurrent from the Changes_List module*/

void newLastMoveSet(Game* game_pointer, int* changes_matrix, int num_of_changes){
	createNodeAfterCurrent(game_pointer->changes_list, SET_CHANGE , changes_matrix, num_of_changes);
}
/*newLastMoveSet adds a "Load Change" to the changes list, with the help of createNodeAfterCurrent from the Changes_List module*/

void newLastMoveLoad(Game* game_pointer){
	createNodeAfterCurrent(game_pointer->changes_list, LOAD_CHANGE , NULL, 0);
}


/*numOfChanges counts the number of changes between the old and the new matrix, both Size*Size matrixes, and returns it*/

int numOfChanges(int* old_matrix, int* new_matrix, int size){
	int i, j, num_of_changes=0, new, old;
	for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				old = old_matrix[i*size + j];
				new = new_matrix[i*size + j];
				if(old!=new){
					num_of_changes+=1;
				}
			}
	}
	return num_of_changes;
}

/*compressChangesTo
 * pre: the arr "destination" is 4 X num_of_changes pre-allocated array ,and size = 4*num_of_changes (num_of_changes means
 * the num of changes between the matrixes).
 * The destination arr is being updated to include all the changes between the old and the new matrix -
 * and for each changed cell, it contains [col, row, old val, new val] */
void compressChangesTo(int* destination, int* old_matrix, int* new_matrix, int size){
	int i, j, new, old, index = 0;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			old = old_matrix[i*size + j];
			new = new_matrix[i*size + j];
			if(old!=new){
				destination[index*4] = j+1;
				destination[index*4 + 1] = i+1;
				destination[index*4 + 2] = old;
				destination[index*4 + 3] = new;
				index+=1;
			}
		}
}
}
