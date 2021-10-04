
#ifndef BT_H_
#define BT_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "GameBoard.h"

/* BT model - is resposible to check how much solution there is to the puzzle from the current puzzle.
 * in the model if we have solution she will print to the user the number of solution there is. if there isn't,
 * she will update the user.
 */

/*For simplicity, the explanations about the functions will be written in the source file*/


bool solveThePuzzleBT(Game *game_pointer);



#endif /* SOLVER_H_ */
