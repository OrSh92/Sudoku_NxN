#include "GameCommands.h"

/*All of these functions have assumptions that the type and range of their arguments are legal (according to guidelines) - and in
 * fact they are called only when this is the case.
 * The return value for all is true iff there was a change in the board, and if there was one they update the changes list
 * (using saveAndMakeChanges function from the Changes_Manager function), and prints the board.
 * Except that, they implement the game commands as requested in the guidelines, regarding to the arguments, prints and returns */


/* gameSet implements the Set command according to guidelines. In a case of a change (the set is of different value than
 * before) it updates the changes list and returns true. Else it returns false. After the set, it prints the board.*/

bool gameSet(Game *game_pointer, int col, int row, int val){
	Mode mode = game_pointer->mode;
	bool check = false;
	int old_val;
	int* change = (int *)malloc(4 * sizeof(int));

	exitIfAllocationFailed(change);

	if(game_pointer->orig_puzzle[(row-1)*game_pointer->blocks + (col-1)]!=0 && mode==SOLVE){
			printf("Error: set to fixed cells in Solve mode is impossible\n");
	}
	else{
		old_val = game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)];
		if(old_val != val){ /*If there was a change*/
			game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)] = val;
			change[0] = col, change[1] = row, change[2] = old_val, change[3] = val;
			newLastMoveSet(game_pointer, change, 1);
			check = true;
			}
      else{
      free(change);
      }
		gamePrint(game_pointer);
	}
	return check;

}

/*gameHint implements the Hint command described in the guidelines. It tries to solve the board using ILP (from the LPSolve module).
 * In a case of success it prints the val for the requested cell. Since the current board needs to stay the same after the command,
 * the return value is always false */

bool gameHint(Game* game_pointer, int col, int row){
	/*int n = game_pointer->blocks;
	int* old_puzzle;*/

	if(game_pointer->orig_puzzle[(row-1)*game_pointer->blocks + (col-1)]!=0){
		printf("Error: cell is fixed\n");
		return false;
	}
	if(game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)]!=0){
		printf("Error: cell is not empty\n");
		return false;
	}

	if(!LP(game_pointer,3,row-1, col-1,0)){
		printf("Error: game is unsolvable\n");
		return false;
	}
	return false;
}

/* gameGuessHint implements the guess hint command according to guidelines, using LP function from the LP Module. In a case of
 * success it prints the legal values of the cell (col,row), else prints that the board is not solvable. Since the current board needs
 *  to stay the same after the command, the return value is always false*/

bool gameGuessHint(Game* game_pointer, int col, int row){
	if(game_pointer->orig_puzzle[(row-1)*game_pointer->blocks + (col-1)]!=0){
		printf("Error: cell is fixed\n");
		return false;
	}
	if(game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)]!=0){
		printf("Error: cell is not empty\n");
		return false;
	}

	if(!LP(game_pointer,5,row-1, col-1,0)){
		printf("Error: board is unsolvable\n");
		/*free(old_puzzle); free(legal_values);*/
		return false;
	}

	return false;
}

/* gameValidate implements the Validate command. It uses ILP function from the LP Module, and prints a message depending
 * on the answer whether there is a valid solution to the board or not. Since the current board needs
 *  to stay the same after the command, the return value is always false*/

bool gameValidate(Game* game_pointer){

	if(!isBoardWithoutErroneous(game_pointer, game_pointer->current_puzzle)){
		printf("Error: the board is erroneous\n");
		return false;
	}

	if(LP(game_pointer,1,0, 0,0)){
		printf("Validation passed: board is solvable\n");
	}
	else{
		printf("Validation failed: board is unsolvable\n");
	}
	return false;
}

/*gameEditNoParam implements the Edit command that doesn't have parameter (and creates 9X9 board). It creates a new game,
 * and therefore there is a change and the return value is true*/

bool gameEditNoParam(Game *game_pointer){
	Game new_game;
	if(game_pointer->need_to_free==true){
		/*if we have already allocated space for the matrixes for a previous game,
		 * we need to free them and to start a new game. The only reason for this function to fail is
		 * an allocation error, in which we exit the whole program. That why we can override an existing game
		 * without knowing if the function succeeded*/
		new_game.mark_errors = game_pointer->mark_errors;
		new_game.changes_list = game_pointer->changes_list;
		freeBoards(game_pointer);
		*game_pointer = new_game;
	}

	game_pointer->rows = 3;
	game_pointer->cols = 3;
	game_pointer->blocks = 9;
	createEmptyBoards(game_pointer);
	game_pointer->mode = EDIT;
	game_pointer->need_to_free=true;

	emptyList(game_pointer->changes_list);
	newLastMoveLoad(game_pointer);
	gamePrint(game_pointer);
	return true;
}

/*gameLoad is used by both gameEditWithParam and gameSolve which implements the Edit with parameter Command
 * and the Solve Command respectively, since the only different between them is the Mode and the check if the fixed
 * board is erroneous. The function uses loadBoard function from the Files Module to insure the file
 * format is right according to the guidelines, and to load it in the right way. It creates a new game and therefore
 * there is a change and the return value is true*/

bool gameLoad(Game *game_pointer, char* path, Mode mode){
	Game new_game;
	Game* new_game_pointer = &new_game;
	bool check = false;
	new_game_pointer->mark_errors = game_pointer->mark_errors;
	new_game_pointer->changes_list = game_pointer->changes_list;

	if(loadBoard(new_game_pointer, path, mode)){
		if(game_pointer->need_to_free){
			freeBoards(game_pointer);
		}
		*game_pointer = new_game;
		game_pointer->mode = mode;
		game_pointer->need_to_free=true;

		emptyList(game_pointer->changes_list);
		newLastMoveLoad(game_pointer);

		gamePrint(game_pointer);
		check = true;
	}
	return check;
}

/*gameEditWithParam implements the Edit Command that requires a parameter. It uses the gameLoad function above,
 * and because of the creation of a new game, there is a change and the function returns true*/

bool gameEditWithParam(Game *game_pointer, char* path){
	return(gameLoad(game_pointer, path, EDIT));
}

/*gameSolve implements the Solve Command function. It uses the gameLoad function above,
 * and because of the creation of a new game, there is a change and the function returns true*/

bool gameSolve(Game *game_pointer, char* path){
	return(gameLoad(game_pointer, path, SOLVE));
}

/*gameMarkErrors implements the Mark Errors command. It changes the mark errors parameter. Since the board
 * stays the same, the return value is false*/

bool gameMarkErrors(Game *game_pointer, int bl){
	game_pointer->mark_errors = bl;
	return false;
}

/*gamePrints implements the Print Board command. It calls the printBoard command from the GameBoard Module. Since the board
 * stays the same, the return value is false*/

bool gamePrint(Game *game_pointer){
	printBoard(game_pointer);
	return false;
}

/*gameAutoFill implements the Auto Fill command and autofills cells according to the guidelines.
 * It checks if the board was changes, and if so - it prints the changes and updates the changes list.
 * Else, it returns false*/

bool gameAutoFill(Game *game_pointer){
	int *tmp_puzzle;
	int n = game_pointer->blocks, options=0, num_to_set=0, i, j, v;
	bool check = false;

	if(isBoardWithoutErroneous(game_pointer,game_pointer->current_puzzle)==false){
		printf("Error: the board is erroneous, can't autofill\n");
	}

	tmp_puzzle = (int *)malloc(n * n * sizeof(int));
	exitIfAllocationFailed(tmp_puzzle);
	copy(tmp_puzzle, game_pointer->current_puzzle, game_pointer->blocks);
	for(i=1;i<n+1;i++){
		for(j=1;j<n+1;j++){
			for(v=1;v<n+1;v++){
				if(isSetValid(game_pointer,j,i,v,game_pointer->current_puzzle)){
					options+=1;
					num_to_set = v;
				}
			}
			if(options==1){
				tmp_puzzle[(i-1)*n + (j-1)] = num_to_set;
			}
			options = 0; num_to_set = 0;
		}
	}

	if(saveAndMakeChanges(game_pointer, game_pointer->current_puzzle, tmp_puzzle, true)){ /*if changes were made*/
		check=true;
	}

	free(tmp_puzzle);
	return check;
}

/*gameSave implements the Save Command using the saveBoard function from the Files Module. Since the board does not changes,
 * the return value is false.*/

bool gameSave(Game *game_pointer, char* path){
	saveBoard(game_pointer, path);
	return false;
}

/*gameUndo implements the Undo Command using the setCurrentPrev from the Changes_List Module and makeChanges
 * from the Changes_Manager Module. It uses the first for one shift left to the pointer of the changes list (if
 * possible), and the second to update the game according to the changes of the board due to the shift. Note that
 * with the two bools that are supplied to makeChanges - that last knows that the shift is a left shift and that a print
 * is required */

bool gameUndo(Game *game_pointer){
	if(!setCurrentPrev(game_pointer->changes_list)){
		printf("Error: no moves to undo\n");
		return false;
	}
	makeChanges(game_pointer, game_pointer->changes_list->current->next->changes_arr,
			game_pointer->changes_list->current->next->arr_len, true, true);
	return true;
}

/*gameRedo implements the Redo Command using the setCurrentNext from the Changes_List Module and makeChanges
 * from the Changes_Manager Module. It uses the first for one shift right to the pointer of the changes list (if
 * possible), and the second to update the game according to the changes of the board due to the shift. Note that
 * with the two bools that are supplied to makeChanges - the last knows that the shift is a right shift and that a print
 * is required */

bool gameRedo(Game *game_pointer){
	if(!setCurrentNext(game_pointer->changes_list)){
		printf("Error: no moves to redo\n");
		return false;
	}
	makeChanges(game_pointer, game_pointer->changes_list->current->changes_arr,
			game_pointer->changes_list->current->arr_len, false, true);
	return true;
}

/*gameReset implements the Rest Command using the setCurrentPrev from the Changes_List Module and makeChanges
 * from the Changes_Manager Module. As requested, it is implemented as a series of Redo commands.
 * It uses the first function to do shift lefts to the pointer of the changes list until the Start (the last Edit/Solve),
 * and the second to update the game according to the changes of the board due to the shift - in every shift. Note that here
 * (unlike undo and redo) we don't print the changes - and therefore the print bool given to makeChanges is false */

bool gameReset(Game *game_pointer){
	bool change = false;
	while(setCurrentPrev(game_pointer->changes_list)){
		makeChanges(game_pointer, game_pointer->changes_list->current->next->changes_arr,
				game_pointer->changes_list->current->next->arr_len, true, false);
		change = true; /*at least one change was happened, and therefore we need to print at the end*/
	}

	gamePrint(game_pointer);
	return change;
}

/*gameExit implements the Exit Command. If the need_to_free field is true - it frees all the allocated stuff - structs and boards.
 * It calls the built-in exit command to terminate the program (and does not return any value)*/

void gameExit(Game* game_pointer){
	if(game_pointer->need_to_free){
		emptyList(game_pointer->changes_list);
		free(game_pointer->changes_list);
		freeBoards(game_pointer);
		free(game_pointer);
	}
	printf("Exiting");
	exit(0);
}

/* gameGuess implements the Guess command according to guidelines, using LP function from the LP Module. In a case of
 * success it updates the current board to the solved one, updates the changes list and returns true.
 * Else, notify that the board is unsolvable and returns false.*/

bool gameGuess(Game* game_pointer, float threshold){ /*!!!!!!!!!!!!!!! need to implement*/
	int n = game_pointer->blocks;
	int* old_puzzle;
	bool check = false;

	if(!isBoardWithoutErroneous(game_pointer, game_pointer->current_puzzle)){
		printf("Error: the board is erroneous\n");
		return false;
	}

	old_puzzle = (int *)malloc(n * n * sizeof(int));
	exitIfAllocationFailed(old_puzzle);
	copy(old_puzzle, game_pointer->current_puzzle, game_pointer->blocks);

	if(!LP(game_pointer,4,0, 0, threshold)){
		printf("Error: the board is unsolvable\n");
		free(old_puzzle);
		return false;
	}

	if(saveAndMakeChanges(game_pointer, old_puzzle, game_pointer->current_puzzle, true)){ /*if changes were made*/
		check=true;
	}
	free(old_puzzle);
	return check;
}

/* gameNumSolutions implements the Num Solutions command according to guidelines, using solveThePuzzle function from the BT Module.
 * Since the board stays the same, it returns false*/


bool gameNumSolutions(Game* game_pointer){
	if(isBoardWithoutErroneous(game_pointer,game_pointer->current_puzzle)==false){
		printf("Error: the board is erroneous\n");
		return false;
	}
	solveThePuzzleBT(game_pointer);
	return false;
}

/* gameGenerate implements the Generate command according to guidelines, using legalRandomXSets and randomXdeletions
 * from the gameBoard Module, and ILPSOLVER from the LPSolve Module .If the board was changed, it updates the changes list
 * and returns true. Else, it returns false*/

bool gameGenerate(Game* game_pointer, int fills, int deletions){
	int n = game_pointer->blocks, iterations = 0;
	int* tmp_puzzle; int* old_puzzle;
	bool check;

	if(numOfEmptyCells(game_pointer)<fills){
		printf("Error: the board doesn't contain enough empty cells\n");
		return false;
	}
	if(!isBoardWithoutErroneous(game_pointer, game_pointer->current_puzzle)){
		printf("Error: generate erroneous board is impossible\n");
		return false;
	}

	tmp_puzzle = (int *)malloc(n * n * sizeof(int));
	old_puzzle = (int *)malloc(n * n * sizeof(int));
	exitIfAllocationFailed(tmp_puzzle); exitIfAllocationFailed(old_puzzle);
	copy(tmp_puzzle, game_pointer->current_puzzle, game_pointer->blocks);
	copy(old_puzzle, game_pointer->current_puzzle, game_pointer->blocks);

	while(!legalRandomXSets(game_pointer, fills) || !LP(game_pointer,2,0, 0,0)){
			copy(game_pointer->current_puzzle, tmp_puzzle, game_pointer->blocks);
			iterations+=1;
			if(iterations==1000){
				printf("Error: could not generate board with %d new cells\n",fills);
				free(tmp_puzzle); free(old_puzzle);
				return false;
			}
	}

	randomXdeletions(game_pointer, game_pointer->blocks*game_pointer->blocks-deletions);

	if(saveAndMakeChanges(game_pointer, old_puzzle, game_pointer->current_puzzle, true)){ /*if changes were made*/
		check=true;
	}
	free(tmp_puzzle); free(old_puzzle);
	return check;
}
