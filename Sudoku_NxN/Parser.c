#include "Parser.h"

/* readAndInterpreterAllCommands is the "run" game of this project. It uses readAndInterpreterCommand function
 * which reads, interpreters and calls the right command from the GameCommands Module (using side-functions of course).
 * The return of readAndInterpreterCommand is true iff there was a change, and using this and the isPuzzlesolved from
 * the gameBoard Module - when the current board is full, it checks if the solution is right. It continues to
 * call readAndInterpreterCommand for every new command, until a fatal error or exit call. It returns nothing.*/

void readAndInterpreterAllCommands(Game *game_pointer){
	bool changed = false;
	printf("--------Sudoku--------\n");
	while(true){
		changed = readAndInterpreterCommand(game_pointer);
		if(changed && game_pointer->mode==SOLVE){
			if(isPuzzlesolved(game_pointer)){
				if(isBoardWithoutErroneous(game_pointer, game_pointer->current_puzzle)){
					printf("Well-done! The puzzle solved successfully\n");
					game_pointer->mode = INIT;
				}
				else{
					printf("Attention! The solution is erroneous\n");
				}

			}
		}
	}
}


 /* readAndInterpreterCommand is the parsing function for a command. Using side-functions like readCommandTo and matchingFunc
  * it reads the command gets the right Command Rep (from the Enum list), and using requiredArgs it
  * gets the number of requested arguments. It uses other functions from this Module to check for errors, like
  * checkAndPrintErrorsWithCommandValidity, checkAndPrintErrorsWithNumOfParam, isParamValid. If all was valid
  * it calls callMatchingFunc, which calls the right function that matches the command, from GameCommands.
  * It returns true iff there was a change in the board */

bool readAndInterpreterCommand(Game *game_pointer){
	char storing_string[COMMAND_MAX_LENGTH + sizeof('\n') + 1];
	char* token;
	char* args[3];
	int count = 0, required_args ,i;
	Command command_rep;
	printf("Please enter a command ["); printMode(game_pointer); printf(" mode]:\n");
	if(!readCommandTo(storing_string, game_pointer)){
		return false; /*ignore*/
	}
	else{
	token = strtok(storing_string, " \t\r\n");
	if(!token){
		return(false); /*ignore*/
	}
	else{
		command_rep = matchingFunc(token, game_pointer->mode);
		if(checkAndPrintErrorsWithCommandValidity(command_rep)){
			return false;
		}
		else{
			required_args = requiredArgs(command_rep);
			token = strtok(NULL, " \t\r\n");
			while(token!= NULL && count<required_args) {
				args[count] = token;
				count+=1;
				token = strtok(NULL, " \t\r\n");
			}

			if(checkAndPrintErrorsWithNumOfParam(&required_args, token, count, &command_rep)){
				return false;
			}

			for(i=0;i<required_args; i++){
				if(!isParamValid(game_pointer, command_rep, args, i)){
					return false;
				}
			}

			return(callMatchingFunc(game_pointer,command_rep,args));
			}
		}
}
}


/* readCommandTo reads a string (representing a command and args), and with length<=COMMAND_MAX_LENGTH+1 (1 for string-end char),
 * and stores them in a string which was supplied to it. It returns false if the command is too long, and true otherwise.*/

bool readCommandTo(char storing_string[], Game* game_pointer){
	bool is_storage_enough;
	bool same_line = true;
	int iterations = 0;
	do {
		char* pointer = fgets(storing_string, COMMAND_MAX_LENGTH + sizeof('\n') + 1, stdin);
		if (pointer == NULL) {
			gameExit(game_pointer);
		}
		else if (strchr(storing_string, '\n')) {
			same_line = false;
		}
		iterations+=1;
	} while(same_line);

	is_storage_enough = iterations<=1;
	if (is_storage_enough) {
		return true;
	}
	return false;
}


/*matchingFunc takes the a string, which supposed to represent a command, and checks if it matches one of the functions.
 * If not, it returns 0. Else, return it returns a unique number matches the command (we could use here the Enum
 * but the use of the indexes is very simple). Then, using this unique number, it calls fromStrToCommand - and return
 * its value - the Enum Rep of the command */

Command matchingFunc(char string[], Mode mode){
	char *functions[17];
	int i=0;
	char *f;
	functions[0] = "set";
	functions[1] = "hint";
	functions[2] = "validate";
	functions[3] = "edit";
	functions[4] = "solve";
	functions[5] = "mark_errors";
	functions[6] = "print_board";
	functions[7] = "autofill";
	functions[8] = "save";
	functions[9] = "undo";
	functions[10] = "redo";
	functions[11] = "reset";
	functions[12] = "exit";
	functions[13] = "guess";
	functions[14] = "generate";
	functions[15] = "num_solutions";
	functions[16] = "guess_hint";

	for(i = 0; i < 17; i++){
		f = functions[i];
		if(strcmp(string,f)==0){
			return fromStrToCommand(i+1, mode);
		}
	}
	return(fromStrToCommand(0, mode)); /*INVAILD_COMMAND*/

}

/*fromStrToCommand takes an int rep of command, given by matchingFunc, and a Mode (represents the current mode of the game)
 * and returns the right matched command from the Enum Command list*/

Command fromStrToCommand(int rep, Mode mode){
	switch(rep)
	{
	    case 1:
	    	if(mode!=INIT){
	    		return SET;
	    	}
	    	return UNAVAILABLE_COMMAND1;
	    case 2:
	    	if(mode==SOLVE){
	    		return HINT;}
	    	return UNAVAILABLE_COMMAND2;
	    case 3:
	    	if(mode!=INIT){
	    	return VALIDATE;
	    	}
	    	return UNAVAILABLE_COMMAND1;
	    case 4:
	    	return EDIT_WITH_PARAMETER;
	    case 5:
	    	return SOLVE_COMMAND;
	    case 6:
	   	    if(mode==SOLVE){
	   	    	return MARK_ERRORS;}
	   	    return UNAVAILABLE_COMMAND2;
	    case 7:
	    	if(mode!=INIT){
	    		return PRINT; }
	    	return UNAVAILABLE_COMMAND1;
	    case 8:
	    	if(mode==SOLVE){
	    		return AUTO_FILL;}
	    	return UNAVAILABLE_COMMAND2;
	    case 9:
	    	if(mode!=INIT){
	    		return SAVE; }
	    	return UNAVAILABLE_COMMAND1;
	    case 10:
	    	if(mode!=INIT){
	    		return UNDO;}
	    	return UNAVAILABLE_COMMAND1;
	    case 11:
	    	if(mode!=INIT){
	    		return REDO;}
	    	return UNAVAILABLE_COMMAND1;
	    case 12:
	    	if(mode!=INIT){
	    		return RESET;}
	    	return UNAVAILABLE_COMMAND1;
	    case 13:
	    	return EXIT;
	    case 14:
	    	if(mode==SOLVE){
	    		return GUESS;}
	    	return UNAVAILABLE_COMMAND2;
	    case 15:
	    	if(mode==EDIT){
	    		return GENERATE;}
	    	return UNAVAILABLE_COMMAND3;
	    case 16:
	    	if(mode!=INIT){
	    		return NUM_SOLUTIONS;}
	    	return UNAVAILABLE_COMMAND1;
	    case 17:
	    	if(mode==SOLVE){
	    		return GUESS_HINT;}
	    	return UNAVAILABLE_COMMAND2;
	}
	return INVAILD_COMMAND;
}


/*requiredArgs gets a Command Representation (from the Command Enum list)
 * and returns the number of its required args. The default is 0.*/

int requiredArgs(Command command_rep){
	if(command_rep==SET){
		return 3;
	}
	if(command_rep==HINT || command_rep==GENERATE || command_rep==GUESS_HINT){
			return 2;
	}
	if(command_rep==EDIT_WITH_PARAMETER || command_rep==SOLVE_COMMAND || command_rep==MARK_ERRORS || command_rep==SAVE
			|| command_rep==GUESS){
		return 1;
	}

	return 0;
}

/* pre: callMatchingFunc is called when command_rep is the rep of the command that the user typed, and args contains
 * the typed arguments - which are all legal in type and range.
 * This function is called after the validity of the command and arguments was checked, and therefore it just
 * calls the right matching function from GameCommands, with casting of some arguments when needed.
 * The return is the matching func - and therefore true iff a change in the board was made*/


bool callMatchingFunc(Game *game_pointer, Command command_rep, char* args[]){
	if(command_rep==SET){
		return(gameSet(game_pointer,atoi(args[0]),atoi(args[1]),atoi(args[2])));
	}
	if(command_rep==HINT){
		return(gameHint(game_pointer,atoi(args[0]),atoi(args[1])));
	}
	if(command_rep==VALIDATE){
		return(gameValidate(game_pointer));
	}
	if(command_rep==EDIT_WITH_PARAMETER){
		return(gameEditWithParam(game_pointer, args[0]));
	}
	if(command_rep==EDIT_NO_PARAMETER){
		return(gameEditNoParam(game_pointer));
	}
	if(command_rep==SOLVE_COMMAND){
		return(gameSolve(game_pointer, args[0]));
	}
	if(command_rep==MARK_ERRORS){
		return(gameMarkErrors(game_pointer, atoi(args[0])));
		}
	if(command_rep==PRINT){
		return(gamePrint(game_pointer));
		}
	if(command_rep==AUTO_FILL){
			return(gameAutoFill(game_pointer));
		}
	if(command_rep==SAVE){
			return(gameSave(game_pointer, args[0]));
		}
	if(command_rep==UNDO){
			return(gameUndo(game_pointer));
		}
	if(command_rep==REDO){
			return(gameRedo(game_pointer));
		}

	if(command_rep==RESET){
		return(gameReset(game_pointer));
	}

	if(command_rep==GUESS){
		return(gameGuess(game_pointer, atof(args[0]))); /*need to add argument*/
	}

	if(command_rep==GENERATE){
		return(gameGenerate(game_pointer,atoi(args[0]),atoi(args[1])));
	}

	if(command_rep==NUM_SOLUTIONS){
		return(gameNumSolutions(game_pointer));
	}

	if(command_rep==GUESS_HINT){
		return(gameGuessHint(game_pointer,atoi(args[0]),atoi(args[1])));
	}

	gameExit(game_pointer);
	return false; /*will never reach here, because there is "exit" in gameExit*/
}

/*isParamValid gets the Command Rep, the array of arguments of the command and an index - meaning it works on a specific argument.
 * It uses convertIfInteger and isFloat to check if the type of the argument is right, and uses getAndSetCommandParamRange
 * in order to calculate the valid range of this specific argument of this command. It returns true iff
 * the argument is in the matching type and range*/

bool isParamValid(Game* game_pointer, Command command_rep, char* args[], int index){
	int range[2];
	int arg_int;
	getAndSetCommandParamRange(game_pointer, command_rep, index+1, range);
	if(range[0]>=0){
		if(!convertIfInteger(args[index], &arg_int) || range[0]>arg_int || arg_int>range[1]){
			printf("Error: Parameter %d (%s) has to be an integer between %d and %d\n",index+1,args[index],range[0],range[1]);
			return false;
		}
	}
	else{
		if(range[0]==-2){
			if(!isFloat(args[index])){
				printf("Error: The parameter (%s) has to be a float\n",args[index]);
				return false;
			}
		}
	}
	return true;
}

/*getAndSetCommandParamRange gets a Command rep and an int contains the number of the argument (it could be the first,
 * second or third). It updates an 2-size array {a,b} representing the [a,b] range. a,b>=0 in all the commands'
 * requirements and therefore -1 used as default for no such range, and -2 for pointing that the parameter
 * needs to be a float to let isParamValid know float casting is needed and not int casting*/

void getAndSetCommandParamRange(Game* game_pointer, Command command_rep, int param_number, int range[]){
	int n = game_pointer->blocks;
	range[0]=-1; range[1]=-1;
	switch(command_rep){
		case GUESS:
			range[0] = -2; range[1] = -2;
			break;
		case MARK_ERRORS:
			range[0] = 0; range[1] = 1;
			break;
		case SET:
			range[0] = (param_number==3) ? 0 : 1;
			range[1] = n;
			break;
		case GENERATE:
			range[0] = (param_number==1) ? 0 : 1;
			range[1] = n*n;
			break;
		case HINT:
			range[0] = 1;
			range[1] = n;
			break;
		case GUESS_HINT:
			range[0] = 1;
			range[1] = n;
			break;
		default:
		  break;
	}
}

/* checkAndPrintErrorsWithNumOfParam returns true iff the amount of supplied parameters [count] doesn't match the requirement.
 * Note the special case of Edit - with a potential parameter*/

bool checkAndPrintErrorsWithNumOfParam(int* required_args_pointer, char* token, int count, Command* command_rep_pointer){
	if(count==*required_args_pointer && token!=NULL){
		if(*command_rep_pointer!=EDIT_WITH_PARAMETER){
			printf("Error: Too many parameters, this command requires exactly %d parameters\n",*required_args_pointer);
		}
		else{
			printf("Error: Too many parameters, this command requires exactly 0 or 1 parameters\n");
		}

		return true;
	}

	if(*command_rep_pointer==EDIT_WITH_PARAMETER && count==0){
	/*Special case because edit has two options - one is with 0	parameters - and we are in this case if we reached here*/
		*command_rep_pointer=EDIT_NO_PARAMETER;
		*required_args_pointer = 0;
	}
	else{
		if(count!=*required_args_pointer){
			printf("Error: Not enough parameters, this command requires exactly %d parameters\n",*required_args_pointer);
			return true;
		}
	}
	return false;
}

/* checkAndPrintErrorsWithCommandValidity returns true iff the command is invalid or unavailable. It calls printUnvailabilityError
 * to print the right unavailability error message*/

bool checkAndPrintErrorsWithCommandValidity(Command command_rep){
	if(command_rep==INVAILD_COMMAND){
		printf("Error: Command name is incorrect\n");
		return true;
	}
	if(command_rep>=UNAVAILABLE_COMMAND1){
		printUnavailabilityError(command_rep);
		return true;
	}

	return false;
}

/*printUnavailabilityError
 * pre: command isn't available in the current mode.
 * The function prints the matching unavailability error message, and returns nothing.*/
void printUnavailabilityError(Command command_rep){
	printf("Error: Unavailable command in the current mode\nThis command is only available in ");
	switch(command_rep)
		{
		    case UNAVAILABLE_COMMAND1:
		    	printf("Edit and Solve modes\n");
		    	break;
		    case UNAVAILABLE_COMMAND2:
		    	printf("Solve mode\n");
		    	break;
		    case UNAVAILABLE_COMMAND3:
		    	printf("Edit mode\n");
		    	break;
		    default:
		    	break;
		}
}

/* convertIfInteger - tries to convert the string to an integer and if it possible and returns true (after converting
 * and saving the converted var). Else, it returns false.
 */

bool convertIfInteger(char* str_rep, int* int_p){
	char* p;
	*int_p = strtol(str_rep, &p, 10);
	if (*p) {
		return false;
	}
	return true;
}

/* isFloat return true iff the string (str_rep) can be converted to float*/

bool isFloat(char* str_rep){
	char* p;
	strtod(str_rep, &p);
	if (*p) {
		return false;
	}
	return true;
}
