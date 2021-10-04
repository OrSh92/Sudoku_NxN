CC = gcc
OBJS = Game_Aux.o Changes_List.o GameBoard.o bt.o LPSolver.o Files.o Changes_Manager.o GameCommands.o Parser.o main.o
EXEC = sudoku
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors


$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Parser.h GameBoard.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h GameCommands.h GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
GameCommands.o: GameCommands.c GameCommands.h Changes_Manager.h Files.h LPSolver.h bt.h GameBoard.h Game_Aux.h
	$(CC) $(COMP_FLAG) -c $*.c
Changes_Manager.o: Changes_Manager.c Changes_Manager.h GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
Files.o: Files.c Files.h LPSolver.h GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
LPSolver.o: LPSolver.c LPSolver.h GameBoard.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
bt.o: bt.c bt.h GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
GameBoard.o: GameBoard.c GameBoard.h Changes_List.h Game_Aux.h
	$(CC) $(COMP_FLAG) -c $*.c
Changes_List.o: Changes_List.c Changes_List.h
	$(CC) $(COMP_FLAG) -c $*.c
Game_Aux.o: Game_Aux.c Game_Aux.h
	$(CC) $(COMP_FLAG) -c $*.c

all: $(EXEC)	
clean:
	rm -f $(OBJS) $(EXEC)