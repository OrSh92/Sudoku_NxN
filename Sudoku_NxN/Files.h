#ifndef FILES_H_
#define FILES_H_

#define MAX_PATH (260)

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "LPSolver.h"
#include "GameBoard.h"

/*The Files Module is responsible for file handling, and therefore contains both save and load boarding functions
 *For simplicity, the explanations about the functions will be written in the source file*/

bool saveBoard(Game* game_pointer, char* path);
bool loadBoard(Game* game_pointer, char* path, Mode mode);
bool loadCells(Game* game_pointer, FILE* file, Mode mode);
bool loadCell(Game* game_pointer, FILE* file, int row, int col, Mode mode);
bool isFileEmpty(FILE* file);
bool isWhiteSpace(char c);

#endif /* FILES_H_ */
