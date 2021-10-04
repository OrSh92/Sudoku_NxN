#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Parser.h"
#include "GameBoard.h"
#include "SPBufferset.h"

int main(int argc, char** argv) {
	Game* game_pointer = createStartGame(); /*create empty game*/

	(void)(argc);
	(void)(argv);
	SP_BUFF_SET();
	srand(time(NULL));

	readAndInterpreterAllCommands(game_pointer); /*run the game*/

	return 0;
}
