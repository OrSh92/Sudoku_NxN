#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "Changes_List.h"
#include "Game_Aux.h"


/*The GameBoard module contains the Game's Structure, including the Modes which are defined here as Enum; as well as
 * functions that are related to the board, for example: functions that check if a board/cell is erroneous, function that returns the
 * number of empty cells in the board and more. These functions are not the Game Commands - that commands have their different
 * module GameCommands, and they use functions from other modules.  For example: gameGenerate in GameCommands which implements
 * that Generate Command, use functions like legalRandomXSets and randomXdeletions from GameBoard module.*/


/*The Enum Mode contains three options - one for each optional mode*/

typedef enum {
	INIT,SOLVE,EDIT
} Mode;


/* *The Game structure contains the following fields:
 *- int cols for the number of cols in a block
 *- int rows for the number of rows in a block
 *- int blocks for the number of blocks
 *- Mode mode for the current Mode
 *- int* current_puzzle - the current board
 *- int* orig_puzzle - "the fixed board" - relevant only in Solve Mode
 *- int mark_errors - the parameter that tell us if we mark errors in printing in Solve mode, or not
 *- bool need_to_free - the parameter that tell us if the boards (current and orig) was already allocated, and therefore we have to
 *	free them before exiting
 *- List* changes_list - the doubly linked list	"undo-redo list"
 */

typedef struct {
	int cols, rows, blocks;
	Mode mode;
	int *current_puzzle;
	int *orig_puzzle;
	int mark_errors;
	bool need_to_free;
	List* changes_list;

} Game;

/*For simplicity, the explanations about the functions will be written in the source file*/

Game* createStartGame();

void createEmptyBoards(Game *game_pointer);
void freeBoards(Game *game_pointer);
bool isBoardWithoutErroneous(Game *game_pointer, int* matrix);
bool isCellValNotErroneous(Game *game_pointer, int col, int row, int* matrix);
bool isSetValid(Game *game_pointer, int col, int row, int val, int* matrix);
bool isSetValidRow(Game *game_pointer, int row, int val, int col, int* matrix);
bool isSetValidCol(Game *game_pointer, int col, int val, int row, int* matrix);
bool isSetValidBlock(Game *game_pointer, int col, int row, int val, int* matrix);
void returnRepCell(Game *game_pointer, int col, int row, int output[2]);
int numOfEmptyCells(Game* game_pointer);
bool isPuzzlesolved(Game* game_pointer);
bool legalRandomXSets(Game* game_pointer, int fills);
bool legalRandomSet(Game* game_pointer, int col, int row);
void randomXdeletions(Game* game_pointer, int deletions);

void printBoard(Game *game_pointer);
void printMode(Game* game_pointer);

#endif /* GAMEBOARD_H_ */
