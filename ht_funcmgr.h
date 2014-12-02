#ifndef HT_FUNCMGR_H_
#define HT_FUNCMGR_H_

#include "valueHandler.h"
#include "dflat.h"

#define MAXCODE 512

/*******************************************************************************
* Function Operations
*/
/* Operations that can be executed */
enum code_ops {HALT=0, SAVE_VAR, JMP_FALSE, JMP_TRUE, LD_VALUE, LD_VAR, CALL_FUNC,
			   ADD, SUB, MUL, DIV, PWR, MOD, DATA,
			   LT, LE, GT, GE, ET, NE, NOT, PRINT};


 /*External Names of executed operations for debugging output */
static char *op_names[] = {"HALT", "SAVE_VAR", "JMP_FALSE", "JMP_TRUE", 
					"LD_VALUE", "LD_VAR", "CALL_FUNC", "ADD", "SUB", "MULT",
					"DIV", "PWR","MOD", "DATA","LT", "LE", "GT", "GE", "ET", 
					"NE", "NOT","PRINT"};


typedef struct _Instruction /*Deals with each operation the function has*/
{
	enum code_ops op;		/* A flag that performs the functions statement */
	S_Value arg;			/* Argument that is used for certain OP codes */

}Instruction;

/*******************************************************************************
* Function Container
*/


typedef struct VARIABLE  	 /* Variable structure format */
{
  char *varName;			 /* Variable name */
  int data_offset;           /* Location of value in function's stackframe*/  
  struct VARIABLE *next;     /* Next simliar variable in the list */
} S_Variable;


typedef struct FUNCTION
{
	char *funcName;          /* Name of function */
	Instruction code[MAXCODE]; /* Code that is ran during its execution */
	int code_offset;         /* Index location of the code instruction */
	int data_offset;         /* Overall data location index */
	int param_offset;		 /* Number of parameters data_offset holds */
	S_Variable *VarData;     /* Function varaiable(s) information */

	struct FUNCTION *next;

}S_Function;


typedef struct HASHTABLE	/* Hash Table structure format */
{
	int size;				/* Size of given hash table */
	S_Function **S_Func;	/* Function structure to requested data*/

} S_HashTable;


/* Create and destroy hash tables*/
S_HashTable *CreateHashTable(int size);
void DestroyHashTable(S_HashTable *hashTable);

/* Modify Hash Table */
int HashKey(S_HashTable *hashTable, char *funcName);
int InsertFunction(S_HashTable *hashTable, char *funcName);
int InsertVariable(S_Function *target, char *varName, int data_offset);

/*Search Hash Table */
S_Function *FindFunction(S_HashTable *hashTable, char *funcName);
S_Variable *FindVariable(S_Function *target, char *varName);
S_Value FindValue(S_Function *target, char *varName);

#endif