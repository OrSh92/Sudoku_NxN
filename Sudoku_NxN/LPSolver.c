
#include "LPSolver.h"
#include <math.h>
#include "gurobi_c.h"
#include <stdlib.h>

/* freeThem - is responsible to run on 2d array and 1d array and free the arrays and the arrays of pointers.
 * the function get:
 * game_pointer
 * ind - 1d array
 * val - 1d array
 * values - 2d array, array of pointers to arrays
 * valuesPossibility - 2d array, array of pointers to arrays
 * t - number of 2d array to release
 * return void, release the arrays
 */

void freeThem(Game *game_pointer,int *ind, double *val,int **values,double **valuesPossibility, int t){
	int i;
	free(ind);
	free(val);
	if(t==0){
		free(values);
		free(valuesPossibility);
	}
	else if(t==1){
		for(i=0;i<game_pointer->blocks*game_pointer->blocks;i++){
			if(values[i]!=NULL){
				free(values[i]);
			}
		}
		free(values);
		free(valuesPossibility);
	}
	else{
		for(i=0;i<game_pointer->blocks*game_pointer->blocks;i++){
			if(values[i]!=NULL){
				free(values[i]);
			}
			if(valuesPossibility[i]!=NULL){
				free(valuesPossibility[i]);
			}
		}
		free(values);
		free(valuesPossibility);
	}
}

/* freeThem2 - is responsible to run on 1d array and free the arrays.
 * the function get:
 * sol - 1d array
 * obj - 1d array
 * vtype - 1d array
 * return void, release the arrays
 */

void freeThem2(double *sol, double *obj, char *vtype ){
	free(sol);
	free(vtype);
	free(obj);
}

/* cellval is responsible to give to each value an index that will be with him
 * from the start of the running of the function LPS and until she finish.
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * row
 * col
 * val
 * return the index for the value in the cell(row,col) that will enter the GUROBI optimization
 */

int cellval(Game *game_pointer, int** values,int row, int col,int val){
	int index=0,a=0,i;
	for(i=0;i<game_pointer->blocks;i++){
		if(values[row*game_pointer->blocks+col][i+1]==val){
			break;
		}
		if(values[row*game_pointer->blocks+col][i+1]!=0){
			a++;
		}
	}
	index= a+values[row*game_pointer->blocks+col][game_pointer->blocks+1];
	return index;
}


/* newIndex - is responsible to update the numbers of values that already been a legal to enter before for each cell
 * the destiny of this function is to make the function cellval to be more effective
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * return void. update the relevant cell
 */

void newIndexs(Game *game_pointer, int **values){
	int i;
	for(i=0;i<game_pointer->blocks*game_pointer->blocks;i++){
		if(i==0){
			values[i][game_pointer->blocks+1]=0;
		}
		else{
			values[i][game_pointer->blocks+1]=values[i-1][0]+values[i-1][game_pointer->blocks+1];
		}
	}
}


/* initialize_allocationFailure - is responsible to check if the allocations work or not
 * the function get: 4 arrays (irrelevant what does the arrays).
 * return false if not, true if yes.
 *
 */

bool initialize_allocationFailure( int *ind, double *val,int **values, double **valuesPossibility){
	if( ind==NULL || val==NULL || values==NULL || valuesPossibility==NULL) return false;
	return true;
}

/* update the values of the solution in ILP running. important only for generate*/

/* solForILP -  is responsible to update the current_puzzle with the result of the GUROBI to the sudoku
 * the function get:
 * game_pointer
 * sol - the array with the result of the solution for the sudoku from the current puzzle that we give to GUROBI
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 *
 * return void, update the current_puzzle with the values that the GUROBI solve.
 */

void solForILP(Game *game_pointer, double *sol,int **values){
	int row,col,k,index;
	for(row=0;row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks; col++){
			if(values[row*game_pointer->blocks +col][0]!=0){
				for(k=0;k<game_pointer->blocks;k++){
					if(values[row*game_pointer->blocks +col][k+1]==0){
						continue;
					}
					index=-1;
					index=cellval(game_pointer,values,row,col,k+1);
					if(index!=-1){
						if(sol[index]==1.0){
							game_pointer->current_puzzle[row*game_pointer->blocks+col]=values[row*game_pointer->blocks +col][k+1];
						}
					}
				}
			}
		}
	}
}

/* update the values of the solution in LP running, important for guess and guess hint */

/* solForLP -  is responsible to update the current_puzzle with the result of the GUROBI to the sudoku
 * the function get:
 * game_pointer
 * sol - the array with the result of the solution for the sudoku from the current puzzle that we give to GUROBI
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 *
 * return void, update the current_puzzle with the values that the GUROBI solve.
 */

void solForLP(Game *game_pointer, double *sol,int **values, double **valuesPossibility){
	int row,col,k,index;
	for(row=0;row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks;col++){
			if(values[row*game_pointer->blocks+col][0]!=0){
				for(k=0;k<game_pointer->blocks;k++){
					valuesPossibility[row*game_pointer->blocks+col][k]=0;
					if(values[row*game_pointer->blocks+col][k+1]==0){
						continue;
					}
					index=-1;
					index=cellval(game_pointer,values,row,col,k+1);
					if(index>=0){
						if(sol[index]>0){
							valuesPossibility[row*game_pointer->blocks+col][k]=sol[index]+values[row*game_pointer->blocks+col][k+1];
						}
					}
				}
			}
		}
	}
}



/* notlegalValuesInCell - is responsible to update the values per cell to if they are legal
 * so they have the value in the array of the cell. and if they not, they will get 0. she do it on base of
 * row and column that is full in the current puzzle with the val that she get.
 * the function is also counting the number of legal values in each cell, and update the per cell.
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * row
 * col
 * val
 * return void, update the values in the correct cells.
 */

void notlegalValuesInCell(Game *game_pointer, int** values,int row,int col,int val){
	int k,i;
	int rep[2];
	for(k=0;k<game_pointer->blocks;k++){
		if(k==col){
			continue;
		}

		if(values[row*game_pointer->blocks +k][val]!=0){
			values[row*game_pointer->blocks +k][val]=0;
			values[row*game_pointer->blocks +k][0]--;
		}

	}

	for(k=0;k<game_pointer->blocks;k++){
		if(k==row){
			continue;
		}
		if(values[k*game_pointer->blocks +col][val]!=0){
			values[k*game_pointer->blocks +col][val]=0;
			values[k*game_pointer->blocks +col][0]--;
		}

	}

	returnRepCell(game_pointer,col+1,row+1,rep);
	for(i = rep[1]-1; i < rep[1]-1+game_pointer->rows; i++){
		for(k = rep[0]-1; k < rep[0]-1+game_pointer->cols; k++){
			if(i==row && k==col){
				continue;
			}
			if(values[i*game_pointer->blocks +k][val]!=0){
				values[i*game_pointer->blocks +k][val]=0;
				values[i*game_pointer->blocks +k][0]--;
			}

		}
	}
}


/*legalvalues - is responsible to check if there is a cell in the current puzzle that have values.
 * if so, she call to notlegalValuesInCell that update the row, col and block of the same cell that we found value.
 * that the same value can't show in the row, col and block.
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * return void.
 */

void legalvalues(Game *game_pointer, int **values){
	int row,col,val;
	for(row=0;row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks;col++){
			if(game_pointer->current_puzzle[row*game_pointer->blocks +col]!=0){
				val=game_pointer->current_puzzle[row*game_pointer->blocks +col];
				notlegalValuesInCell(game_pointer,values,row,col,val);
			}
		}
	}
}

/* numbOfLegal - is responsible to update the int numofvals to the number of legal value that can place in the current_puzzle.
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * numofvals - a number of legal values in the current_puzzle
 * return void, update numofvals.
 */

void numbOfLegal(Game *game_pointer, int **values, int *numofvals){
	int vals=0,i;
	for(i=0;i<game_pointer->blocks*game_pointer->blocks;i++){
		vals+=values[i][0];
	}
	*numofvals=vals;
}


/* HindILP is implement the gameHint function. With the solution with the ILP the function is getting a row and a column
 * and return the value that return from the ILP solution. The function is not changing the current puzzle.
 * she only print to the user the value in the specific cell.
 * *the function get:
 * game pointer
 * sol - is the solution of gurobi to the sudoku.
 * row and col that the user ask for.
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * return void, print to the user the hint in the cell (x,y)
 */

void HintILP(Game *game_pointer,double *sol, int row, int col, int **values){
	int i,index;
	for(i=0;i<game_pointer->blocks;i++){
		if(values[row*game_pointer->blocks+col][i+1]==0){
			continue;
		}
		index=-1;
		index=cellval(game_pointer,values,row,col,i+1);
		if(index>=0){
			if(sol[index]==1.0){
				printf("The value found for (col=%d, row=%d) is %d\n", col+1, row+1, i+1);
			}
		}
	}
}

/* randomNumber - is responsible to create a random value between low and high.
 * the implement base on the explanation of moshe in the model.
 * the function get:
 * double low
 * double high
 * return int value, a random number between the range.
 */

int randomNumber(double low, double high){
	double valid, range, value;
	valid= high-low;
	range= RAND_MAX/valid;
	value = rand()/range + low;
	return value;

}

/* LPguess - is implement the gameHint function. With the solution of the LP, the function is getting
 * a threshold and update the current puzzle on base of the cell with values with the probability bigger from the
 * threshold. if there at least 2 values the function return random value between all the possibility
 * the function get:
 * game_pointer
 * threshold
 * valuesPossibility - array of pointers to a arrays of doubles. the values in the array is the possibility of each value to each cell.
 * return void, update the current_puzzle with the correct value to the correct cells.
 */

void LPguess(Game *game_pointer, float threshold, double **valuesPossibility){
	int row,col,k,thevalue,numOfPossibility;
	double remainder, random, sumOfprobability,range;
	for(row=0;row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks;col++){
			sumOfprobability=0,numOfPossibility=0;
			if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){
				continue;
			}
			for(k=0;k<game_pointer->blocks;k++){
				if(valuesPossibility[row*game_pointer->blocks+col][k]==0 || !isSetValid(game_pointer,col+1,row+1,k+1, game_pointer->current_puzzle)){
					continue;
				}
				remainder=valuesPossibility[row*game_pointer->blocks+col][k]-k-1;
				if(remainder>=threshold){

					numOfPossibility++;
					sumOfprobability+=remainder;
				}
			}
			if(numOfPossibility==1){
				for(k=0;k<game_pointer->blocks;k++){
					if(valuesPossibility[row*game_pointer->blocks+col][k]==0 || !isSetValid(game_pointer,col+1,row+1,k+1, game_pointer->current_puzzle)){
						continue;
					}
					remainder=valuesPossibility[row*game_pointer->blocks+col][k]-k-1;
					if(remainder>=threshold){
						game_pointer->current_puzzle[row*game_pointer->blocks+col]=k+1;
					}
				}
			}
			else if(numOfPossibility>1){
				random=randomNumber(0.0,sumOfprobability);
				range=1.0;
				thevalue=0;
				for(k=0;k<game_pointer->blocks;k++){
					if(valuesPossibility[row*game_pointer->blocks+col][k]==0 || !isSetValid(game_pointer,col+1,row+1,k+1, game_pointer->current_puzzle)){
						continue;
					}
					remainder=valuesPossibility[row*game_pointer->blocks+col][k]-k-1;
					if(remainder>=random){
						if(remainder<=range){
							range=remainder;
							thevalue=k+1;
						}
					}
				}
				game_pointer->current_puzzle[row*game_pointer->blocks+col]=thevalue;
			}
		}
	}
}

/* LPGuessHint - is implement the gameGuessHint function. the function is responsible to print the all legal values
 * to the cell that the user wanted. and only values with possibility bigger then zero.
 * the function get:
 * game_pointer
 * row
 * column
 * valuesPossibility - array of pointers to a arrays of doubles. the values in the array is the possibility of each value to each cell.
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * return void, the function is print all the values of the cell that have possibility bigger then 0
 */

void LPGuessHint(Game *game_pointer, int row, int col, double **valuesPossibility,int **values){
	double remainder;
	int i;
	printf("The legal values for the cell (col=%d, row=%d) are:",col+1,row+1);
	for(i=0;i<game_pointer->blocks;i++){
		if(values[row*game_pointer->blocks +col][i+1]==0){
			continue;
		}
		remainder=valuesPossibility[row*game_pointer->blocks +col][i]-i-1;
		if(remainder>0){
			printf(" %d with the score of %f",i+1,remainder);
		}
	}
	printf("\n");
}

/* initialize_array - is responsible the get the 2d array values and initialize the array. each cell with a pointer
 * will point to array with all the options values in the game, and update each array which values are legal
 * to the cell with the pointer that parallel to the cell in current_puzzle.
 * the function get:
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * return void, update the values array.
 */

void initialize_array(Game *game_pointer,int **values){
	int row,col,k;
	for(row=0;row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks;col++){
			if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){
				values[row*game_pointer->blocks+col][0]=0;
				for(k=1;k<game_pointer->blocks+1;k++){
					values[row*game_pointer->blocks+col][k]=0;
				}
				continue;
			}
			values[row*game_pointer->blocks+col][0]=game_pointer->blocks;
			for(k=1;k<game_pointer->blocks+1;k++){
				values[row*game_pointer->blocks+col][k]=k;
			}
		}
	}
}

/* addObjVtype - is responsible to update the values in the arrays obj and vtype dependent on ILP or LP.
 * the function get:
 * game_pointer
 * vtype - array of the type of the values, can be binary or continuous dependent if we use ILP or LP Respectively
 * obj - array with the
 * function - an int that determine if it will result with LP or ILP.
 * numofvals - an int that said how much variable we had
 * return void, update the 2 arrays.
 */

void addObjVtype(Game *game_pointer, char *vtype, double *obj, int function, int numofvals ){
	int i;
	if (1==function || function==2 || function==3){
		for(i=0;i<numofvals;i++){
			vtype[i]=GRB_BINARY;
			obj[i]=0;
		}
	}
	else {
		for(i=0;i<numofvals;i++){
			vtype[i]=GRB_CONTINUOUS;
			obj[i]=rand()%(game_pointer->blocks*game_pointer->blocks) +1;
		}
	}
}

/* addConstraint - is responsible to sent to the GUROBI the constraint for solve the sudoku. base on
 * constraint to row, column and block.
 * the function get:
 * env - the environment that we work on
 * model - the model that we adding the constraint.
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * ind - an array that we add the index of the value in the constraint
 * val - an array that we add the coefficient of the value in the constraint
 * return int, -1 if there is error 1 if not.
 */

int addConstraint(GRBenv* env, GRBmodel* model, Game *game_pointer, int **values, int *ind, double *val){
	int row,col,value,specVal,cnt,error,error2,i_b,j_b;

	/* value need to appear only one time in each col*/
	for(value=1; value<game_pointer->blocks+1;value++){
		  for(col=0;col<game_pointer->blocks;col++){
			  cnt=0;
			  for(row=0;row<game_pointer->blocks;row++){
				  specVal=0;
				  if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){/* the cell is already filled with value*/
					  continue;
				  }
				  if(values[row*game_pointer->blocks+col][value]==0){
					  /* if the value in this cell is not 0,
					  it mean that the value is already appears in the column, row or block */
					  continue;
				  }
				  specVal =cellval(game_pointer,values,row,col,value);
				  ind[cnt]=specVal;
				  val[cnt]=1.0;
				  cnt++;
			  }
			  if(cnt>0){
				  error=0,error2=0;
				  error = GRBaddconstr(model, cnt, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
				  error2 = GRBaddconstr(model, cnt, ind, val, GRB_GREATER_EQUAL, 1.0, NULL);
				  if (error || error2) {
					  printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					  return -1;
				  }
			  }
		  }
	  }
	/* value need to appear only one time in each row*/
	for(value=1; value<game_pointer->blocks+1; value++) {
		for (row = 0; row < game_pointer->blocks; row++) {
			cnt=0;
			for (col=0; col<game_pointer->blocks; col++) {
				specVal = 0;
				if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){/* the cell is already filled with value*/
					continue;
				}
				if(values[row*game_pointer->blocks+col][value]==0){
					/* if the value in this cell is not 0,
					  it mean that the value is already appears in the column, row or block */
					continue;
				}
				specVal =cellval(game_pointer,values,row,col,value);
				ind[cnt] = specVal;
				val[cnt] = 1.0;
				cnt++;
			}
			if(cnt>0){
				error=0,error2=0;

				error = GRBaddconstr(model, cnt, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
				error2 = GRBaddconstr(model, cnt, ind, val, GRB_GREATER_EQUAL, 1.0, NULL);
				if (error || error2) {
					printf("ERROR %d 3nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return -1;
				  }
			  }
	      }
	  }
	/* value need to appear only one time in each block*/
	for(value=1; value<game_pointer->blocks+1;value++){
		for(i_b=0;i_b<game_pointer->cols;i_b++){
			for(j_b=0;j_b<game_pointer->rows;j_b++){
				cnt=0;
				for(row=i_b*game_pointer->rows;row<(i_b+1)*game_pointer->rows;row++){
					for(col=j_b*game_pointer->cols;col<(j_b+1)*game_pointer->cols;col++){
						specVal=0;
						if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){/* the cell is already filled with value*/
							continue;
						}
						if(values[row*game_pointer->blocks+col][value]==0){
							/* if the value in this cell is not 0,
				  	  	  	  it mean that the value is already appears in the column, row or block */
							continue;
						}
						specVal =cellval(game_pointer,values,row,col,value);
	 					ind[cnt]=specVal;
	 					val[cnt]=1.0;
	 					cnt++;
					}
	 			  }
				if(cnt>0){
					error=0,error2=0;
					error = GRBaddconstr(model, cnt, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
					error2 = GRBaddconstr(model, cnt, ind, val, GRB_GREATER_EQUAL, 1.0, NULL);
	 	 		    if (error || error2) {
	 	 		    	printf("ERROR %d 4nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	 	 			 	return -1;
	 	 			  }
	 			  }
	 		  }
	 	  }
	   }
	return 1;
}


/* addConstraintForCell - is responsible to sent to the GUROBI the constraint for each cell to solve the sudoku.
 * the function get:
 * env - the environment that we work on
 * model - the model that we adding the constraint.
 * game_pointer
 * values - a 2d array with all the legal values that can be in the puzzle that
 * isn't in the current_puzzle. each cell have his list of the legal value for him.
 * ind - an array that we add the index of the value in the constraint
 * val - an array that we add the coefficient of the value in the constraint
 * function - an int that should determine if we using LP or ILP
 * return int, -1 if there is error 1 if not.
 */

int addConstraintForCell(GRBenv* env,GRBmodel* model, Game *game_pointer, int **values, int *ind, double *val,int function){
	int row,col,value,cnt,error,error2,specVal;

	for(row=0; row<game_pointer->blocks;row++){
		for(col=0;col<game_pointer->blocks;col++){
			if(game_pointer->current_puzzle[row*game_pointer->blocks+col]!=0){/* the cell is already filled with val*/
				continue;
			}
			cnt=0;
			for(value=1;value<game_pointer->blocks+1;value++){
				specVal=0;
				if(values[row*game_pointer->blocks+col][value]==0){ /* if the value in this cell is not 0,
				   it mean that the value is already appears in the column, row or block */
					continue;
				  }
				specVal =cellval(game_pointer,values,row,col,value);
				if (function==4 || function==5){
					double bbb=1;
					error=0,error2=0;
					error = GRBaddconstr(model, 1, &specVal, &bbb, GRB_LESS_EQUAL, 1.0, NULL);
					error2 = GRBaddconstr(model, 1, &specVal, &bbb, GRB_GREATER_EQUAL, 0.0, NULL);
					if (error || error2) {
						printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
						return -1;
					}
				}
				ind[cnt]=specVal;
				val[cnt]=1;
				cnt++;
			  }
			  if(cnt>0){
				  error=0,error2=0;
				  error = GRBaddconstr(model,cnt, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
				  error2 = GRBaddconstr(model,cnt, ind, val, GRB_GREATER_EQUAL, 1.0, NULL);
				  if (error || error2) {
					  printf("ERROR %d 11st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					  return -1;
				  }
			  }
		  }
	  }
	return 1;
}

/* ILPs - is responsible on all the run of GUROBI
 *
 * the function get:
 * game_pointer
 * function - to determine if we use ILP or LP and which function as call the LPSolver.
 *
 *
 * threshold - a float number
 * return -1 if there error, 1 O.W.
 */

int ILPs(Game *game_pointer,int function,int x, int y, float threshold)
{

  GRBenv *env=NULL;
  GRBmodel *model=NULL;
  double *sol=NULL;
  int *ind=(int*) malloc(game_pointer->blocks*sizeof(int));
  double *val=(double*) malloc(game_pointer->blocks*sizeof(double));
  double *obj= NULL;
  char *vtype=NULL;
  int **values = (int**) malloc(game_pointer->blocks*game_pointer->blocks*sizeof(int*));
  double **valuesPossibility=(double**)malloc(game_pointer->blocks*game_pointer->blocks*sizeof(double*));
  int numofvals=0,m, optimstatus=0,error=0;

  if(initialize_allocationFailure(ind, val,values,valuesPossibility)==false){
	  printf("Error: an allocation error has occurred, exiting...\n");
	  if(ind!=NULL){
		  free(ind);
	  }
	  if(val!=NULL){
		  free(val);
	  }
	  if(values!=NULL){
		  free(values);
	  }
	  if(valuesPossibility!=NULL){
		  free(valuesPossibility);
	  }
	  return -1;
  }

  GRBfreeenv(env);
  error = GRBloadenv(&env, NULL);
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,0);
	  GRBfreeenv(env);
  	  return -1;
  }

  error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,0);
	  GRBfreeenv(env);
  	  return -1;
  }

  /* Create an empty model named "mip1" */
  error = GRBnewmodel(env, &model, NULL, 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
  	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
  	  freeThem(game_pointer,ind,val,values,valuesPossibility,0);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
  	  return -1;
  }

  if(initialize_allocationFailure(ind, val,values,valuesPossibility)==false){
	  printf("Error: an allocation error has occurred, exiting...\n");
	  freeThem(game_pointer,ind,val,values,valuesPossibility,0);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  for(m=0;m<game_pointer->blocks*game_pointer->blocks;m++){
	  values[m]=(int*)malloc((game_pointer->blocks+2)*sizeof(int));
	  if(values[m]==NULL){
		  printf("Error: an allocation error has occurred, exiting...\n");
		  freeThem(game_pointer,ind,val,values,valuesPossibility,1);
		  GRBfreemodel(model);
		  GRBfreeenv(env);
		  return -1;
	  }
  }
  for(m=0;m<game_pointer->blocks*game_pointer->blocks;m++){
	  valuesPossibility[m]=(double*)malloc(game_pointer->blocks*sizeof(double));
	  if(valuesPossibility[m]==NULL){
		  printf("Error: an allocation error has occurred, exiting...\n");
		  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
		  GRBfreemodel(model);
		  GRBfreeenv(env);
		  return -1;
	  }
  }

  initialize_array(game_pointer,values);
  legalvalues(game_pointer,values);
  newIndexs(game_pointer,values);
  numbOfLegal(game_pointer, values, &numofvals);
  sol=(double*) malloc(numofvals*sizeof(double));
  obj = (double*) malloc(numofvals*sizeof(double));
  vtype=(char*) malloc(numofvals*sizeof(char));

  if(sol==NULL || obj==NULL || vtype==NULL){
	  printf("Error: an allocation error has occurred, exiting...\n");
	  if(sol!=NULL){
		  free(sol);
	  }
	  if(obj!=NULL){
		  free(obj);
	  }
	  if(vtype!=NULL){
		  free(vtype);
	  }
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }
  addObjVtype(game_pointer,vtype, obj, function, numofvals);
  /* add variables to model */
  error = GRBaddvars(model, numofvals, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  /* Change objective sense to maximization */
  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  /* update the model - to integrate new variables */
  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }
  m=addConstraintForCell(env,model, game_pointer, values, ind,val,function);
  if(m!=1){
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  m=addConstraint(env,model, game_pointer, values, ind, val);
  if(m!=1){
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  /* Optimize model - need to call this before calculation */
  error = GRBoptimize(model);
  if (error) {
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  /* Get solution information */
  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) {
	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, numofvals, sol);
  if (error) {
	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  return -1;
  }

  /* solution found */
  if (optimstatus == GRB_OPTIMAL) {
	  printf("solution as been found\n");
  }

  /* no solution found */
  else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
    freeThem(game_pointer,ind,val,values,valuesPossibility,2);
    freeThem2(sol,obj,vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return -1;
  }

  /* error or calculation stopped */
  else {
	  /*return there is unsolved*/
    printf("Optimization was stopped early\n");
    freeThem(game_pointer,ind,val,values,valuesPossibility,2);
    freeThem2(sol,obj,vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return -1;
  }

  if(function==1){
	  GRBfreemodel(model);
	  GRBfreeenv(env);
	  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
	  freeThem2(sol,obj,vtype);
	  return 1;
  }
  else{
	  if(function==2 || function==3){
		  if(function == 2){ /* for generate function*/
			  solForILP(game_pointer,sol,values);
		  }
		  else if (function==3){ /*for hint function*/
			  HintILP(game_pointer, sol, x, y, values);
		  }
	  }
	  else{
		  solForLP(game_pointer, sol, values, valuesPossibility);
		  if(function==4){ /* for guess function*/
			  LPguess(game_pointer, threshold, valuesPossibility);
		  }
		  else if (function==5){ /*for guess hint function*/
			  LPGuessHint(game_pointer, x, y, valuesPossibility,values);
		  }

	  }
  }

  /* IMPORTANT !!! - Free model and environment*/

  GRBfreemodel(model);
  GRBfreeenv(env);
  freeThem(game_pointer,ind,val,values,valuesPossibility,2);
  freeThem2(sol,obj,vtype);
  sol=NULL;ind=NULL; val=NULL; vtype=NULL; obj=NULL;
  return 1;
}

/* LP - is responsible to return if the GUROBI found a solution the the current puzzle.
 * the function get:
 * game_pointer
 * function - an int number that is Stands for which function call the LPSolver
 * row
 * col
 * threshold
 * return true if there is solution to the current board, false o.w.
 */

bool LP(Game *game_pointer,int function,int row, int col,float threshold){
	int valOfsol=0;
	valOfsol=ILPs( game_pointer, function, row,col, threshold);
	if(valOfsol==1){
		return true;
	}
	return false;
}
