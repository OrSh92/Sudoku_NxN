#ifndef LPSOLVER_H_
#define LPSOLVER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "GameBoard.h"
#include "gurobi_c.h"

/* LPSolver - This model is responsible to solve the current board. the model have 2 options to solve the board.
 * when the possibility is binary or continuous in the range of [0,1].
 * the model is work with 6 function: save, validate, generate hint, guess and guess hint. for each one of them
 * he will behave like the function should be executed. in the model he will print or update the current puzzle
 * base on the function that use him.
 * For simplicity, the explanations about the functions will be written in the source file */

bool ILPSOLVER(Game* game_pointer);
bool ILPSOLVERNoUpdate(Game* game_pointer);
bool LPSOLVER(Game* game_pointer, float f);
bool LPAlllegalVals(Game* game_pointer, int* out_arr, int* arr_size, int col, int row);
bool LP(Game *game_pointer,int function,int row, int col,float threshold);
#endif /* LPSOLVER_H_ */
