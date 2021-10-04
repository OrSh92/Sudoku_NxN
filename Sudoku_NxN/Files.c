#include "Files.h"

/*saveBoard - gets a path of a file and save the game board to this file in the requested format and according
 * to guidelines. It returns true iff the board was saved successfully*/

bool saveBoard(Game* game_pointer, char* path){
	int i,j, n = game_pointer->blocks;
	FILE *fptr_out;
	fptr_out = fopen(path, "w");
	if(fptr_out==NULL){ /*path isn't fine*/
		printf("Error: writing to file was unsuccessful. Make sure the path is correct\n");
		fclose(fptr_out);
		return false;
	}
	else{
		if(game_pointer->mode==EDIT){
			if(!isBoardWithoutErroneous(game_pointer, game_pointer->current_puzzle)){
				printf("Error: erroneous boards may not be saved\n");
				fclose(fptr_out);
				return false;
			}
			if(!LP(game_pointer,1,0, 0,0)){
				printf("Error: unsolvable boards may not be saved\n");
				fclose(fptr_out);
				return false;
			}
		}

		fprintf(fptr_out, "%d %d\n", game_pointer->rows,game_pointer->cols);
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if((game_pointer->mode==EDIT && game_pointer->current_puzzle[i*n + j]!=0) ||
					game_pointer->orig_puzzle[i*n + j]!=0){
					fprintf(fptr_out, "%d.", game_pointer->current_puzzle[i*n + j]);
				}
				else{
					fprintf(fptr_out, "%d", game_pointer->current_puzzle[i*n + j]);
				}
				if(j!=n-1){
					fprintf(fptr_out, " ");
				}
				else{
					fprintf(fptr_out, "\n");
				}
			}
			}

	}
	fclose(fptr_out);
	printf("Game was saved\n");
	return true;
}

/*loadBoard, as mentioned in GameCommands is used to load boards for both Edit and Solve Command
 * It takes the path of the file and the mode, and checks for errors (which changes a bit between
 * the two commands). If the file is valid and in the right format, it updates the current board and in Solve command also the
 * orig(=fixed) board according to the file. For this, it uses the loadCells function.
 * It returns true iff the game board was loaded successfully*/

bool loadBoard(Game* game_pointer, char* path, Mode mode){
	int n=0, m=0, supplied_dims = 0;
	bool check=false;
	FILE *fptr_in;
	fptr_in = fopen(path, "r");
	if(fptr_in==NULL){ /*file wasn't found*/
		printf("Error: the file couldn't be read. Make sure the path is correct\n");
    return false;
	}
	else{
		supplied_dims = fscanf(fptr_in, " %d %d ", &m, &n);
		if(supplied_dims != 2 || m<=0 || n<=0) {
			printf("Error: The first two arguments have to be positive integers\n");
		}
		else{
			game_pointer->rows = m, game_pointer->cols = n, game_pointer->blocks = m*n;
			createEmptyBoards(game_pointer);

			if(!loadCells(game_pointer, fptr_in, mode)){
				freeBoards(game_pointer);
			}
			else{
				if(!isFileEmpty(fptr_in)){
					printf("Error: too much values\n");
				}
				else{
					if(mode==SOLVE && !isBoardWithoutErroneous(game_pointer, game_pointer->orig_puzzle)){
						printf("Error: starting board, of fixed cells, is already erroneous\n");
					}
					else{
					check=true;
					}
				}
			}
		}
	}
	fclose (fptr_in);
	return check;
}

/*loadCells - tries to load to the game board the cells appearing in the file, in the use of loadCell.
 * It returns true iff the game board was loaded successfully*/

bool loadCells(Game* game_pointer, FILE* file, Mode mode) {
	int i = 0, j = 0;
	for(i = 0; i < game_pointer->blocks; i++){
		for (j = 0; j < game_pointer->blocks; j++){
			if(!loadCell(game_pointer, file, i+1, j+1, mode)){
				return false;
			}

		}
	}
	return true;
}

/*loadCells tries to load a specific cell, that its value is being read from the file, to the board game.
 * It uses the row and col arguments in order to know where to exactly to set this val in the board.
 * It uses the Mode to know if the orig (=fixed) board needs to be updated as well (relevant only to Solve).
 * It returns true iff the cell was loaded successfully*/

bool loadCell(Game* game_pointer, FILE* file, int row, int col, Mode mode){
	char c = '0';
	int num_of_param = 0, val = 0;
	num_of_param = fscanf(file, " %d%c ", &val, &c);

	if(num_of_param == 2) {
		if(c=='.'){
			if(val==0){
				printf("Error: 0 can not be fixed\n");
			}
			else{
				if(val<0 || val>game_pointer->blocks){
					printf("Error: fixed value (%d) is outside range [1,%d]\n",val,game_pointer->blocks);
				}
				else{
					game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)] = val;
					if(mode==SOLVE){
						game_pointer->orig_puzzle[(row-1)*game_pointer->blocks + (col-1)] = val;
					}
					return true;
				}
			}
		}
		else{
			if(isWhiteSpace(c)){
				if(val<0 || val>game_pointer->blocks){
					printf("Error: value (%d) is outside of range [0,%d]\n",val,game_pointer->blocks);
				}
				else{
					game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)] = val;
					return true;
				}
			}
			else{
				printf("Error: incorrect format - contains %c\n", c);
			}
		}
	}
	else{
		if(num_of_param == 1 && row==game_pointer->blocks && col==game_pointer->blocks){
				game_pointer->current_puzzle[(row-1)*game_pointer->blocks + (col-1)] = val;
				return true;
		}
		else{
			num_of_param = fscanf(file, " %c ", &c);
			if(num_of_param==1){
				if(c=='.'){
				printf("Error: dot (.) must be immediately after an integer\n");
				}
				else{
					if(!isWhiteSpace(c)){
						printf("Error: incorrect format - contains %c\n", c);
					}
				}
			}
			else{
			printf("Error: missing values\n");
			}
			}
		}
	return false;
}


/*isFileEmpty checks if the file is empty and returns the result*/

bool isFileEmpty(FILE* file) {
	char c = '0';
	int is_free = 0;
	is_free = fscanf(file, " %c", &c);
	if(is_free == EOF){
		return true;
	}
	return false;
}

/*isWhiteSpace returns true iff the char is a white space*/

bool isWhiteSpace(char c){
	return(c==' ' || c=='\t' || c=='\r' || c=='\n');
}
