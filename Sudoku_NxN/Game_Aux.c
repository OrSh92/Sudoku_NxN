#include "Game_Aux.h"

/*createZeroMatrix creates a sizeXsize zero matrix using the setZeroes function*/


int* createZeroMatrix(int size){
	int* matrix = (int *)malloc(size * size * sizeof(int));
	exitIfAllocationFailed(matrix);
	setZeroes(matrix, size);
	return matrix;
}

/*setZeroes takes a sizeXsize matrix and sets every cell in it to 0*/

void setZeroes(int *puzzle,int size){
	int i,j;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; j++){
			puzzle[i*size + j] = 0;
		}
	}

}

/*exitIfAllocationFailed
 * pre arr is a pointer for an array that was allocated (using malloc or calloc).
 * If the arr is NULL (meaning that the allocation failed) the program exits (not cleanly). Otherwise, it does nothing.
 * */

void exitIfAllocationFailed(int* arr){
	if(arr==NULL){
		printf("Error: an allocation error has occurred, exiting...\n");
		exit(1);
	}
}

/*printDashes - used by printCurrentState for the right printing board format*/
void printDashes(int blocks, int rows_f_block){
	int i;
	for(i=0;i<4*blocks+rows_f_block+1;i++){
		if (i==4*blocks+rows_f_block){
			printf("-\n");
		}
		else{
			printf("-");
		}
	}
}

/*printCurrentState - prints the current board of a game, without getting a direct Game instance. The current board and
 * the fixed board, with the num of blocks, and rows and cols for a block - are enough to print the right board - with only
 * one more detail missing: which cells are erroneous. The conditions matrix supply the latter
 * The reason for including this function in the Module, was totally conceptual and it was a lot more
 * easier to keep it in GameBoard Module. But as said before, such a heavy technical function has to be placed in this module*/

void printCurrentState(int* current, int* orig, int blocks, int rows_f_block, int cols_f_block, int* conditions){
	int i,j;
	for(i=1;i<blocks+1;i++)
	{
		if((i-1)%rows_f_block==0){
			printDashes(blocks, rows_f_block);
		}
		printf("|");
		for(j=1;j<blocks+1;j++)
		{
			if(orig[(i-1)*blocks + j-1]!=0){
				printf(" %2d.",current[(i-1)*blocks + j-1]);
			}
			else{
				if(current[(i-1)*blocks + j-1]!=0){
					if(conditions!=NULL && !conditions[(i-1)*blocks + j-1]){
						printf(" %2d*",current[(i-1)*blocks + j-1]);
					}
					else{
						printf(" %2d ",current[(i-1)*blocks + j-1]);
					}
				}
				else{
					printf("    ");
				}
			}
			if (j>0){
				if((j%cols_f_block==0) &&(j==blocks)){
					printf("|\n");
				}
				else if(j%cols_f_block==0){
					printf("|");
				}
			}
		}
	}
	printDashes(blocks, rows_f_block);
}

/*Copy - takes 2 SizeXSize arrays (their size is supplied as well), and copy the content of the second array to the first*/

void copy(int *arr, int *arr2, int size){
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			arr[i*size + j]=arr2[i*size + j];
		}
	}
}


