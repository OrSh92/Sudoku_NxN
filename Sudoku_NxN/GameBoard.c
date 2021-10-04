#include "GameBoard.h"

/* createStartGame - The function creates a new game, that still lacks the dims and boards which will be added after the user's command.
 it allocates place for the game structure and initialize the other fields.
 It returns a pointer to the game*/

Game* createStartGame(){
	Game *game_pointer = (Game*) calloc(1, sizeof(Game));
	List* list_pointer = createNewList();

	game_pointer->mode=INIT;
	game_pointer->mark_errors = 1;
	game_pointer->need_to_free=false;
	game_pointer->changes_list = list_pointer;

	return game_pointer;
}

/* createEmptyBoards:
 * pre: the function is called when the "blocks" field of the game is already updated (after Edit or Solve Command).
 * The function gets a Game pointer, and using the blocks field - it updates the Game board fields to zero blocks*blocks
 * matrixes (using createZeroMatrix from Game_Aux). It does not return anything.
 */

void createEmptyBoards(Game *game_pointer){
	int n = game_pointer->blocks;
	game_pointer->current_puzzle = createZeroMatrix(n);
	game_pointer->orig_puzzle = createZeroMatrix(n);
}


/* createEmptyBoards:
 * pre: the function is called after the game board fields were already allocated.
 * It gets a Game pointer, and free the current_puzzle and orig_puzzle fields*/

void freeBoards(Game *game_pointer){
	free(game_pointer->current_puzzle);
	free(game_pointer->orig_puzzle);
}

/*isBoardWithoutErroneous:
 * pre: the matrix size has the same size as the Game board.
 * The function gets a matrix (supposed to be current_puzzle or orig_puzzle) and using isCellValNotErroneous,
 * that check for every cell if it is not Erroneous, it return true iff the matrix doesn't contain erroneous cells
 * The matrix argument is need so we can use this function for both current and "fixed" puzzle - which both needed
 * for some function.
 * The game pointer is needed as some functions which will soon be called need the blocks, rows and cols fields*/

bool isBoardWithoutErroneous(Game *game_pointer, int* matrix){
	int i, j;
	for(i = 1; i < game_pointer->blocks+1; i++){
		for(j = 1; j < game_pointer->blocks+1; j++)
			if(isCellValNotErroneous(game_pointer,j,i, matrix)==false){
				return false;
			}
		}
		return true;
}

/*pre: (col,row) is in the matrix range, and like the prev function the matrix size has the same size as the Game board.
 * The function uses isSetValid - a function that checks if a set is legal (in terms of row, col and block validity). It calls that
 * function in the following way: it supplies the current value of the cell as "set value". In this way we don't need two
 * implementations for validity of new and current cell.
 * It return true iff the (col,row) cell of the matrix is not erroneous*/

bool isCellValNotErroneous(Game *game_pointer, int col, int row, int* matrix){
	return(isSetValid(game_pointer, col, row, matrix[(row-1)*game_pointer->blocks+(col-1)], matrix));
	}


/* pre: like the prev func 0 (col,row) is in the matrix range, and the matrix size has the same size as the Game board
 * isSetValid checks if the set is legal according to soduko's rules, by checking for the same val in the same col, row and val.
 * If it does not exists in any of them, or the val is 0 (meaning it is a blank cell), it returns true; else false.
 * It uses 3 functions: isSetValidRow, isSetValidCol, isSetValidBlock */

bool isSetValid(Game *game_pointer, int col, int row, int val, int* matrix){
	return(val==0 || (isSetValidRow(game_pointer,row,val, col, matrix)
			&& isSetValidCol(game_pointer,col,val, row, matrix) && isSetValidBlock(game_pointer,col,row,val, matrix)));
	}

/* pre: like the prev func 0 (col,row) is in the matrix range, and the matrix size has the same size as the Game board
	isSetValidRow searches for the same val in the same row in the matrix. It returns true iff the val is not
	already in the row*/

bool isSetValidRow(Game *game_pointer, int row, int val, int col, int* matrix){
	int i = 0;
	for(i = 0; i < game_pointer->blocks; i++){
		if(i!=(col-1) && matrix[(row-1)*game_pointer->blocks+i]==val){
			return false;
		}
	}
	return true;
	}

/* pre: like the prev func 0 (col,row) is in the matrix range, and the matrix size has the same size as the Game board
	isSetValidCol searches for the same val in the same col in the current puzzle. It returns true iff the val is not
	already in the col*/

bool isSetValidCol(Game *game_pointer, int col, int val, int row, int* matrix){
	int i = 0;
	for(i = 0; i < game_pointer->blocks; i++){
		if(i!=(row-1) && matrix[i*game_pointer->blocks + (col-1)]==val){
			return false;
		}
	}
	return true;
	}

/* pre: like the prev func 0 (col,row) is in the matrix range, and the matrix size has the same size as the Game board
	isSetValidCol searches for the same val in the same block in the current puzzle. It returns true iff the val is not
	already in the block. It uses the func returnRepCell which return the top-left cell of the block
	and from which the walk on the block begins in this function*/

bool isSetValidBlock(Game *game_pointer, int col, int row, int val, int* matrix){
	int rep_cell[2];
	int i, j;
	returnRepCell(game_pointer, col,row,rep_cell);
	for(i = rep_cell[1]; i < rep_cell[1]+game_pointer->rows; i++){
		for(j = rep_cell[0]; j < rep_cell[0]+game_pointer->cols; j++)
			if((row!= i || col!=j) && matrix[(i-1)*game_pointer->blocks + (j-1)]==val){
				return false;
		}
	}
	return true;
	}

/*returnRepCell gets the row and the col of the cell and with the game pinter, that it gets as well, it extracts the rows-in-blocks and
 * cols-in-blocks of the board. It uses the four int vars to to calculate the most top-left
 * cell of the block (used in isSetValidBlock). It returns two-length array which represents the [col,row] of the returned cell*/

void returnRepCell(Game *game_pointer,int col, int row, int output[2]){
	double temp;
	temp = 1+ game_pointer->cols*(floor((col-1)/game_pointer->cols));
	output[0]=(int) temp;
	temp = 1 + game_pointer->rows*(floor((row-1)/game_pointer->rows));
	output[1]=(int) temp;
}

/*numOfEmptyCells:
 * pre: current board is not null.
 * The function calculates the number of empty cell in the current puzzle*/

int numOfEmptyCells(Game* game_pointer){
	int i, j, n = game_pointer->blocks, count = 0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(game_pointer->current_puzzle[i*n + j]==0){
				count+=1;
			}
		}
	}
	return count;
}

/*The function return true if and only if the puzzle is solved, not necessarily correctly.
 * It uses numOfEmptyCells, and is being used after changes in the game*/

bool isPuzzlesolved(Game* game_pointer){
	return(numOfEmptyCells(game_pointer)==0);
}


/*legalRandomXSets
 * pre - there are at least X empty cells in the current board
 * The function tries to randomize legal vals for X [=fills] cells, using legalRandomSet. For every unempty and new cell that
 * was randomized, and using legalRandomSet, it tries to get and set a random legal val for the set. If a cell was
 * not set successfully with vals (legalRandomSet=false) it returns false.
 * If all total X [=fills] cells were successfully set with vals - it returns true. The updates of the board is done with legalRandomSet.
 * Note that we assume here that if legalRandomXSets=false the current board is switched back to the state before the set (outside
 * of this function). */

bool legalRandomXSets(Game* game_pointer, int fills){
	int remaining = fills, n = game_pointer->blocks, col, row;
	while(remaining>0){
		col = 1 + rand()% n;
		row = 1 + rand()% n;

		/*we need to choose X empty cells - that is why when we get nonempty cell, we just try again. Note that it is not the
		 * case of a cell without legal options - which requires to start filling from the beginning. In that case we will return false.
		 */
		if(game_pointer->current_puzzle[(row-1)*n + (col-1)]==0){
			if(legalRandomSet(game_pointer, col, row)){
				remaining-=1;
			}
			else{
				return false;
			}
		}
	}
	return true;
}

/*legalRandomSet checks for validates val for the cell (col, row) in the current board using isSetValid.
 * If there is at least one legal val, it set a randomize legal val in the same cell in the current board; and return true.
 * Else it returns false;
 */

bool legalRandomSet(Game* game_pointer, int col, int row){
	int n = game_pointer->blocks, i, legal_options = 0;
	int* values = (int *)malloc(n * sizeof(int));
	bool check = false;
	exitIfAllocationFailed(values);
	for(i=1; i<=n; i++){
		if(isSetValid(game_pointer, col, row, i, game_pointer->current_puzzle)){
			values[legal_options] = i;
			legal_options+=1;
		}
	}
	if(legal_options>0){
		game_pointer->current_puzzle[(row-1)*n + (col-1)] = values[rand()% legal_options];
		check = true;
	}

	free(values);
	return check;
}

/*randomXdeletions
 * pre - there are at least X nonempty cells in the current board.
 * The function randomizes X [=deletions] nonempty cells and set them to 0. It returns nothing*/

void randomXdeletions(Game* game_pointer, int deletions){
	int remaining = deletions, n = game_pointer->blocks, col, row;
	while(remaining>0){
		col = 1 + rand()% n;
		row = 1 + rand()% n;

		if(game_pointer->current_puzzle[(row-1)*n + (col-1)] != 0){
			game_pointer->current_puzzle[(row-1)*n + (col-1)] = 0;
			remaining-=1;
		}
	}
}

/*printBoard prints the current board according to the guidelines. It uses printCurrentState from Game_Aux
 * which prints the right format with the use of the condition matrix. This matrix states for every cell if it is
 *erroneous (checks with the use of isCellValNotErroneous) - and in this way Game_Aux can prints the right format
 *without needing to include the gameBoard file, since it is wrong conceptually. It returns nothing. */

void printBoard(Game *game_pointer){
	int i, j, blocks = game_pointer->blocks;
	int* conditions;
	bool condition = (game_pointer->mode==EDIT || game_pointer->mark_errors==1);
	if(condition==false){
		printCurrentState(game_pointer->current_puzzle, game_pointer->orig_puzzle,
				blocks, game_pointer->rows,  game_pointer->cols, NULL);
	}
	else{
		conditions = (int*)malloc(blocks * blocks * sizeof(int));
		exitIfAllocationFailed(conditions);
		for(i=1; i<=blocks; i++){
			for(j=1; j<=blocks; j++){
				condition = isCellValNotErroneous(game_pointer, j, i, game_pointer->current_puzzle);
				conditions[(i-1)*blocks + j-1] = condition;
			}
		}
		printCurrentState(game_pointer->current_puzzle, game_pointer->orig_puzzle,
						blocks, game_pointer->rows,  game_pointer->cols, conditions);
		free(conditions);

	}
}

/*The function printMode prints the current mode of the game.*/

void printMode(Game* game_pointer){
	Mode mode = game_pointer->mode;
	switch(mode)
	{
		case INIT:
			printf("INIT");
			break;
		case SOLVE:
			printf("SOLVE");
			break;
		case EDIT:
			printf("EDIT");
			break;
	}
}
