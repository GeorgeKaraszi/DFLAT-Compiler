/******************************************************************************
* Description: The idea for devising a runtime stack for each function comes 
*			   from a mixture of Anthony A. Aaby bison stack implementation 
*			   and the way C compiles code into assembly operations. The primus
*			   is simple: Each expression bison finds, devises stack OP codes to 
*			   setup and execute once the function has been interpreted.
* Credit:
*	Author: Anthony A. Ababy
*	Book:Compiler Contruction
*
*/

#include "ht_funcmgr.h"

/*******************************************************************************
* View the stack throughout the life span of the running program. 
* Ideal for debugging.
*/
void StackView(S_Value *stack, char *eval, int top)
{
	int index = 0;

	printf("\n\nRunning:%s\n", eval);
	printf("Stack Before Run\t TOP=%d\n", top);
	for(index = 0; index < 6; index++)
	{
		printf("%d: Type=%dNumber=", index ,stack[index].valtype);
		PrintValue(stack[index]);
	}

	printf("\n\n");
}

/*******************************************************************************
* Executes a given function based on its code operations. Push stack is a way 
* of passing parameters to other functions in an instance of a function call.
*/

int execute_function(S_HashTable *hashtable, char *funcName, S_Value *pushStack)
{
	Instruction ic 	    	  = {0};  /* Current instruction being executed */
	S_Value stack[512]  	  = {{0}};/* Stack frame for the given function */
	S_Value *calleStack 	  = NULL; /* Passing parameters to a called function */
	S_Function *exec, *called = NULL; /* Current and called function executing */
	int pc = 0, top		      = 0;    /* Stack location arguments*/
	int index , index2        = 0;    /* General index for array's */

	if((exec = FindFunction(hashtable, funcName)) == NULL)
	{
		printf("Could not find (%s)! \n", funcName);
		return -1;
	}

	/* Checks to see if their are any parameters being passed */
	if(pushStack != NULL)
	{
		/*Copies parameter values to the top of the stack for use later */
		for(index = 0, top = exec->param_offset-1;top >= 0; index++, top--)
			stack[top] = pushStack[index];
	
	}

	/*StackView(stack, op_names[ic.op],top);

	printf("All OP codes of the running function(%s):\n", funcName);
	for(index = 0; index < exec->code_offset; index++)
	{
		printf("%s\n", op_names[exec->code[index].op]);
	}*/
	

	do
	{
		ic = exec->code[pc++];
		/*StackView(stack, op_names[ic.op],top);*/
		switch(ic.op)
		{
			case DATA:      /* Tells the stack where executing data begins at */
				top = ic.arg.x.ival;
				break;
			case LD_VAR:    /*Loads a varaible from the very top of stack */
				stack[++top] = stack[ic.arg.x.ival];
				break;
			case LD_VALUE:  /*Loads inital value based on the given argument */
				stack[++top] = ic.arg;
				break;
			case SAVE_VAR:  /* Saves value to the very top of the stack */
				stack[ic.arg.x.ival] = stack[top--];
				break;
			case CALL_FUNC: /*Calls a function, sets up parameters as well*/
				called = FindFunction(hashtable, ic.arg.x.sval);
				calleStack = calloc(called->param_offset, sizeof(S_Value));

				for(index = 0; index < called->param_offset; index++)
				{
					calleStack[index] = stack[top-index];
				}

				execute_function(hashtable, called->funcName, calleStack);
				free(calleStack);
				break;
			case JMP_TRUE:  /*Change array index position if condtion was true */
				pc = ic.arg.x.ival;
				break;
			case JMP_FALSE: /*Change array index position if condition was false */
				if(stack[top].x.bval == false)
					pc = ic.arg.x.ival;
				top--;
				break;
			case PRINT:     /* Print value */
				PrintValue(stack[top--]);
				break;
			case ADD:
				stack[top-1] = Add_S(stack[top-1], stack[top]);
				top--;
				break;
			case SUB:
				stack[top-1] = Sub_S(stack[top-1], stack[top]);
				top--;
				break;
			case MUL:
				stack[top-1] = Mult_S(stack[top-1], stack[top]);
				top--;
				break;
			case DIV:
				stack[top-1] = Div_S(stack[top-1], stack[top]);
				top--;
				break;
			case MOD:
				stack[top-1] = Mod_S(stack[top-1], stack[top]);
				top--;
				break;
			case PWR:
				stack[top-1] = PowerRise_S(stack[top-1], stack[top]);
				top--;
				break;
			case LT:
			case LE:
			case GE:
			case GT:
			case ET:
			case NE:
				stack[top-1] = CompareValues_S(stack[top-1], stack[top], ic.op);
				top--;
				break;
			case NOT:
				stack[--top] = NotExp_S(stack[top]);
				break;
			default:
			break;
		}

	}while(ic.op != HALT);  /* Stop program when halt signal hits */


	return 0;
}