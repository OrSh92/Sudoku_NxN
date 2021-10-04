#ifndef GAME_AUX_H_
#define GAME_AUX_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/* Game_Aux Module contains functions, mainly technical and not Game-specific, that are used by the Game Modules –
 *  GameBoard and GameCommands, like createZeroMatrix and copy (of matrixes).*/

/* For simplicity, the explanations about the functions will be written in the source file*/

int* createZeroMatrix(int size);
void setZeroes(int *puzzle,int size);
void exitIfAllocationFailed(int* arr);
void printDashes(int blocks, int rows_f_block);
void printCurrentState(int* current, int* orig, int blocks, int rows_f_block, int cols_f_block, int* conditions);
void copy(int *arr, int *arr2, int size);

#endif /* GAME_AUX_H_ */
