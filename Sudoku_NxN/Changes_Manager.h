#ifndef CHANGES_MANAGER_H_
#define CHANGES_MANAGER_H_

#include <stdbool.h>
#include <stdio.h>

#include "GameBoard.h"

/*The Changes_Manager is responsible for handling with Board Changes - both compressing and decompressing of "changes matrixes"
 * from the changes list; while each matrix represents changes made to the board due to one user's command.
 * If the changes are not because of redo/undo/reset command - it extracts the changes that were made, compresses them
 * to a matrix and adds them to the changes list.
 * For a redo/undo/reset - it extracts the changes need to be made from the changes matrix, and change the board accordingly*/

/*For simplicity, the explanations about the functions will be written in the source file*/

bool saveAndMakeChanges(Game* game_pointer, int* old_matrix, int* new_matrix, bool to_print);
void makeChanges(Game* game_pointer, int* changes_matrix, int num_of_changes, bool back, bool to_print);
void newLastMoveSet(Game* game_pointer, int* changes_matrix, int num_of_changes);
void newLastMoveLoad(Game* game_pointer);
int numOfChanges(int* old_matrix, int* new_matrix, int size);
void compressChangesTo(int* destination, int* old_matrix, int* new_matrix, int size);

#endif /* CHANGES_MANAGER_H_ */
