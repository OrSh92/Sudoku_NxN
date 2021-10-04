#ifndef GAMECOMMANDS_H_
#define GAMECOMMANDS_H_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "Changes_Manager.h"
#include "Files.h"
#include "LPSolver.h"
#include "bt.h"
#include "Game_Aux.h"
#include "GameBoard.h"

/*The GameCommands module contains all the commands the user can command. Because of this, it needs to include most of the
 * head files of the project (since their main job is done in functions from other files). However, including them all in
 * one Module is important, mainly because the Parser module need to call them all, but also conceptually
 * For simplicity, the explanations about the functions will be written in the source file*/

bool gameSet(Game *game_pointer, int col, int row, int val);
bool gameHint(Game* game_pointer, int col, int row);
bool gameGuessHint(Game* game_pointer, int col, int row);
bool gameValidate(Game* game_pointer);
bool gameEditNoParam(Game *game_pointer);
bool gameLoad(Game *game_pointer, char* path, Mode mode);
bool gameEditWithParam(Game *game_pointer,char* path);
bool gameSolve(Game *game_pointer, char* path);
bool gameMarkErrors(Game *game_pointer, int bl);
bool gamePrint(Game *game_pointer);
bool gameAutoFill(Game *game_pointer);
bool gameSave(Game *game_pointer, char* path);
bool gameUndo(Game *game_pointer);
bool gameRedo(Game *game_pointer);
bool gameReset(Game *game_pointer);
void gameExit(Game *game_pointer);
bool gameGuess(Game* game_pointer, float threshold);
bool gameNumSolutions(Game* game_pointer);
bool gameGenerate(Game* game_pointer, int fills, int deletions);

#endif /* GAMECOMMANDS_H_ */
