/*
	Author:George Karaszi
	Date:12-1-2014
	Subject: Programing Lauaguages A5

	Description: Create syntax for the flex and bison so that a user can 
	type equtions and produce results. If input are a set of two strings, 
	then concatenate the string set if they are being added together. Otherwise 
	keep track to datatypes via a series of function method that modify the 
	data type structure and perform nessarcy house keeping to produce corrective
	results.

*/

%{
	#include "dflat.h"
	#include "CG.c"
	#include "RunFunction.h"

	static S_HashTable *functionHT;	 /* Varible obtained result/Modifier */
	S_Function *targetFunction;      /* Function that is being developed */
	/*int yydebug=1;*/               /*Uncomment to see runtime debugging */


	typedef struct _lbs
	{
		int jmp_true;
		int jmp_false;
	}lbs;

	struct _lbs *newLabelRecord()
	{
		return malloc(sizeof(lbs));
	}
%}

/*
	Generate include file with symbols and types
*/
%code requires {
	#include "valueHandler.h"
	#include "ht_funcmgr.h"
}
%defines

%union
{
	char *string;
	S_Value value;
	struct _lbs *label;
}

%token <string> ID
%token <value> STRING
%token <value> INTEGER
%token <value> FLOAT
%token <label> IF

%token LBRACE
%token RBRACE
%token SEMICOLON
%token PRINT_TOKEN
%token LCURB
%token RCURB
%token EQUALS
%token PLAYLIST END COMMA CALL IF_END ELSE FUNC_END

%left '+' '-' '*' '/' '%'
%right '^'

token '<' '>' ET_TOK GE_TOK LE_TOK NE_TOK '!'

%start program

%%


program : PLAYLIST ID {
						if(functionHT == NULL)
							functionHT = CreateHashTable(128);

						InsertFunction(functionHT, $2);
						targetFunction = FindFunction(functionHT, $2);
						TargetFunction(targetFunction);
					  }
		  parameters  {
		  				gen_code(DATA, GenerateValue(false, INT_T, 0));
		  			  }
		  statements 
		  FUNC_END   {
		  				gen_code(HALT, GenerateValue(false, UNKNOWN));
		  				gen_code_build_stack();
		  			 }
		  	program

		|
		  ;

parameters : LBRACE parameter ID RBRACE 
					{
					   InsertVariable(targetFunction, $3, data_location(true));
					}
		   | LBRACE RBRACE
		   ;
parameter : 
		  | parameter ID COMMA
		  			{
		  			   InsertVariable(targetFunction, $2, data_location(true));
		  			}
		  ;

statements :
		   | condition statements
		   | statement SEMICOLON statements
		   | statement error SEMICOLON program
		   ;

condition : IF exp   {
						$1 = newLabelRecord();
						$1->jmp_false = reserve_loc();
					 }
		  statements {
		  				$1->jmp_true = reserve_loc();
		  			 }
		  ELSE		 {
		  				gen_code_loc($1->jmp_false, JMP_FALSE, gen_label());
		  			 }
		  statements
		  IF_END	 {
		  				gen_code_loc($1->jmp_true, JMP_TRUE, gen_label());
		  			 }
		  ;

statement 
		: ID EQUALS exp 
					 {
					 	if(FindValue(targetFunction, $1).valtype == UNKNOWN)
					 		InsertVariable(targetFunction, $1, data_location(false));

						gen_code(SAVE_VAR, FindValue(targetFunction, $1));
					 }
		| ID FuncArguments
					{
						gen_code(CALL_FUNC, GenerateValue(false, STR_T, $1));
					}
		| PRINT_TOKEN exp
				    {
				    	gen_code(PRINT, GenerateValue(false, UNKNOWN));
				    }
		;

FuncArguments : LBRACE FuncArgument exp RBRACE
			  | LBRACE RBRACE
			  ;
FuncArgument :
			 | FuncArgument exp COMMA
			 ;
exp : LBRACE exp RBRACE
	| exp '+' exp		{gen_code(ADD, GenerateValue(false, UNKNOWN));}
	| exp '-' exp		{gen_code(SUB, GenerateValue(false, UNKNOWN));}
	| exp '*' exp		{gen_code(MUL, GenerateValue(false, UNKNOWN));}
	| exp '/' exp		{gen_code(DIV, GenerateValue(false, UNKNOWN));}
	| exp '%' exp		{gen_code(MOD, GenerateValue(false, UNKNOWN));}
	| exp '^' exp 		{gen_code(PWR, GenerateValue(false, UNKNOWN));}
	| exp '<' exp		{gen_code(LT, GenerateValue(false, UNKNOWN));}
	| exp LE_TOK exp	{gen_code(LE, GenerateValue(false, UNKNOWN));}
	| exp '>' exp		{gen_code(GT, GenerateValue(false, UNKNOWN));}
	| exp GE_TOK exp	{gen_code(GE, GenerateValue(false, UNKNOWN));}
	| exp ET_TOK exp	{gen_code(ET, GenerateValue(false, UNKNOWN));}
	| exp NE_TOK exp	{gen_code(NE, GenerateValue(false, UNKNOWN));}
	| '!' exp			{gen_code(NOT, GenerateValue(false, UNKNOWN));}
	| INTEGER			{gen_code(LD_VALUE, GenerateValue(true, INT_T, $1));}
	| FLOAT				{gen_code(LD_VALUE, GenerateValue(true, FLOAT_T, $1));}
	| STRING			{gen_code(LD_VALUE, GenerateValue(true, STR_T, $1));}
	| ID               	{
							if(FindValue(targetFunction, $1).valtype == UNKNOWN)
								InsertVariable(targetFunction, $1, data_location(false));

							gen_code(LD_VAR, FindValue(targetFunction, $1));
						}
	;
%%

/*******************************************************************************
* Displays error messages that arise during the parsing of the program
*/

extern void yyerror(char *s) 
{

  printf( "Error: %s\n", s);
}

/*******************************************************************************
* Starting point of the DFLAT compiler/interpreter
*/

int main(int argv, char **argc)
{
	
	if(argv > 1)
		yyin = fopen(argc[1], "r");
	else
		yyin = fopen("./testfile.dat", "r");

	if(!yyin)
	{
		printf("Could not open testfile.dat try stdin instead!\n");
		yyin = stdin;
	}
	else
		printf("Reading testfile!\n");

	yyparse();

	if(functionHT == NULL)
		printf("Function HashTable is null?\n");
	else
	{
		printf("\n\n%sExecuting Program%s%s\n\n", "\033[44m", "\033[0m", 
			   "\033[92m");
		execute_function(functionHT, "Main", NULL);
	}

	printf("\n%s%sWe have reached the EOF...%s\n" ,"\033[0m", "\033[44m", 
			   "\033[0m");

	return 0;
}