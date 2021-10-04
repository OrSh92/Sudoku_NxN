#ifndef PARSER_H_
#define PARSER_H_

#define COMMAND_MAX_LENGTH (256)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameCommands.h"
#include "GameBoard.h"

/*The Parser module is responsible for parsing the commands, given by the user. It checks for invalid and unavailable command
 * errors, as well as for errors with the amount of parameter or with the type or range of a parameter.
 * If all is valid - it calls the function that matches the command - in GameCommands Module.
 * The Enum Command is used by functions in this Module to get details on the specific command - for example how many arguments is
 * needed, what are the arguments types and ranges, when it is unavailable and more. The three UNAVAILABLE_COMMANDs
 * of this Enum are different from each other in the modes that they are unavailable in. Both INVAILD and UNAVILABLE used
 * only for Printing issues and of course don't call any of GameCommands functions

For simplicity, the explanations about the functions will be written in the source file*/

typedef enum {
	SET,HINT,VALIDATE,EDIT_WITH_PARAMETER,EDIT_NO_PARAMETER, SOLVE_COMMAND, MARK_ERRORS, PRINT, AUTO_FILL,SAVE, UNDO, REDO, RESET,EXIT,
	GUESS, GUESS_HINT, GENERATE, NUM_SOLUTIONS, INVAILD_COMMAND, UNAVAILABLE_COMMAND1, UNAVAILABLE_COMMAND2, UNAVAILABLE_COMMAND3
} Command;


void readAndInterpreterAllCommands(Game *game_pointer);
bool readAndInterpreterCommand(Game *game_pointer);
bool readCommandTo(char storing_string[], Game* game_pointer);
bool interpeterCommand();
Command matchingFunc(char string[], Mode mode);
Command fromStrToCommand(int rep, Mode mode);
int requiredArgs(Command command_rep);
bool callMatchingFunc(Game *game_pointer, Command command_rep, char* args[]);
bool isParamValid(Game* game_pointer, Command command_rep, char* args[], int index);
void getAndSetCommandParamRange(Game* game_pointer, Command command_rep, int param_number, int range[]);
bool checkAndPrintErrorsWithNumOfParam(int* required_args_pointer, char* token, int count, Command* command_rep_pointer);
bool checkAndPrintErrorsWithCommandValidity(Command command_rep);
void printUnavailabilityError(Command command_rep);

bool convertIfInteger(char* str_rep, int* int_p);
bool isFloat(char* str_rep);

#endif /* PARSER_H_ */
