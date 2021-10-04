#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bt.h"

/* The Stack structure contains the following fields:
 * int top the number of element there is in the stack - 1.
 * int capacity the size of the max stack that we can use.
 * int *array - the array is to simulate a stack.
 */

typedef struct Stack {
	int top;
	int* array;
	int capacity;
} Stack;


/* stackCreating - is responsible to create a array, to simulate the stack, the size of the board.
 * and to initialize the top field, the capacity field and the array field.
 * the function get:
 * capacity - an int the implement the size of the stack.
 * return stack if the allocation is not NULL.
 */

struct Stack* stackCreating(int capacity){
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
	stack-> top = -1;
	stack->capacity=capacity;
	stack->array = (int*)malloc(capacity*sizeof(int));
	if(stack->array==NULL)
		{
			printf("Error: initialize_allocation has failed\n");
			/**/
			return false;
		}
	return stack;
}

/* push - is responsible to add an item to the stuck
 * the function get:
 * stack
 * item - a int value
 * return void, update the stack and increase the stack size(the top)
 */

void push(struct Stack* stack, int item){
	if(stack->top==stack->capacity-1)	{return;}
	stack->array[++stack->top]=item;
}

/* pop - is responsible to remove the item in the top on the stuck.
 * the function get:
 * stack
 * return void, update the stack and decrease the stack size(the top)
 */

void pop(struct Stack* stack){
	if (stack->top==-1)	{return;}
	stack->top--;

}

/* value - is responsible to check what value is in the top of the stack.
 * the function get:
 * stack
 * return if not empty the value in the top o.w 0.
 */

int value(struct Stack* stack){
	if (stack->top!=-1){
		return stack->array[stack->top];
	}
	else {
		return 0;
	}
}


/* goingBackWord - is responsible to going back in the matrix that simulator the sudoku puzzle.
 * the running backword is to at least 2 item. because if we going back at the end of the sudoku puzzle
 * the last item is always be the same.
 * the function get:
 * game_pointer
 * row
 * col
 * matrix - a 1d int array that simulator the sudoku puzzle.
 * stack
 * arr - return the column and row that we going back to, if we at the end of the puzzle(out size of him) it will return
 * that we should end the run of the backtracking function.
 * return void, update the row and column that we are in it after the running and update the cells that are empty in the current puzzle
 * that we pass at the running backword.
 */

void goingBackWord(Game *game_pointer,int row, int col,int *matrix,struct Stack *stack, int arr[3]){
	while(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){
		col--;
		if(col==-1 && row!=0){
			col=game_pointer->blocks -1;
			row--;
		}
		if(col==-1 && row==0){
			arr[2]=1;
			break;
		}
	}
	matrix[row*game_pointer->blocks+col]=0;
	pop(stack);
	col--;
	if(col==-1 && row!=0){
		col=game_pointer->blocks -1;
		row--;
	}
	while(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){
			col--;
			if(col==-1 && row!=0){
				col=game_pointer->blocks -1;
				row--;
			}
			if(col==-1 && row==0){
				arr[2]=1;
				break;
			}
		}
	arr[0]=row,arr[1]=col;
}

/* deterministic_BT - is responsible to check how much solution there is for the current puzzle.
 * the function get:
 * game_pointer
 * stack
 * matrix -  a 1d int array that simulator the sudoku puzzle.
 * cnt - the number of solution that we should update
 * return void, update the number of cnt.
 */

void deterministic_BT(Game *game_pointer,struct Stack *stack, int *matrix, int *cnt){
	int row=0,col=0,backword=0,val,b=0;
	int arr[3]={0};
	while(true){
		if(col==game_pointer->blocks && row!=game_pointer->blocks-1){
			row++;
			col=0;
		}
		else if(col==game_pointer->blocks && row==game_pointer->blocks-1){
			*cnt+=1;/* we finish, so we have solution*/
			col--;
			goingBackWord(game_pointer, row,col,matrix,stack,arr);
			if(arr[2]==1){
				return;
			}
			row=arr[0],col=arr[1];
			b=value(stack);
			matrix[row*game_pointer->blocks+col]=0;
			pop(stack);
		}
		else if (col==-1 && row!=0){
			col=game_pointer->blocks-1;
			row--;
		}
		else if (col==-1 && row==0){
			return;
		}
		if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){
			if(backword==1){
				col--;
			}
			else{
				col++;
			}
		}
		else{
			if(b==game_pointer->blocks){
				b=value(stack);
				pop(stack);
				matrix[row*game_pointer->blocks+col]=0;
				col--;
			}
			else{
				for(val=b+1;val<game_pointer->blocks+1;val++){
					matrix[row*game_pointer->blocks+col]=val;
					if(isSetValid(game_pointer,col+1,row+1,val,matrix)){
						push(stack,val);
						col++;
						b=0;
						backword=0;
						break;
					}
					if(val>=game_pointer->blocks){
						matrix[row*game_pointer->blocks+col]=0;
						b=value(stack);
						pop(stack);
						backword=1;
						col--;
						if (col==-1 && row!=0){
							col=game_pointer->blocks-1;
							row--;
						}
					}
				}
			}
		}
	}
}

/* solveThePuzzle - is resposible to return if there is solution. if so it will print the number of solution,
 * o.w it will update the user.
 * the function get:
 * game_pointer
 * return bool, true if there is solution, false o.w.
 */

bool solveThePuzzleBT(Game *game_pointer){
	int *matrix;
	int cnt=0;
	struct Stack *stack=stackCreating(game_pointer->blocks*game_pointer->blocks);
	matrix=createZeroMatrix(game_pointer->blocks);
	copy(matrix, game_pointer->current_puzzle, game_pointer->blocks);
	deterministic_BT( game_pointer, stack, matrix, &cnt);
	if(cnt>0){
		printf("the number of solution is: %d\n",cnt);
		free(matrix);
		free(stack->array);
		free(stack);
		return true;
	}
	printf("there is no solution found to the puzzle in that current situation\n");
	free(stack->array);
	free(stack);
	free(matrix);
	return false;
}
