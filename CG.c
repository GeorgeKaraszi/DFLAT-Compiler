
#include "ht_funcmgr.h"


int code_offset;
int data_offset;
int param_offset;
Instruction global_code[MAXCODE];
Instruction global_var[MAXCODE];

/*******************************************************************************
* Getter & Setter of the current function we are working with
*/
S_Function *TargetFunction(S_Function *src)
{
	static S_Function *target;

	if(src != NULL)
	{
		code_offset = 0;
		data_offset = 0;
		target = src;
	}

	return target;
}

/*******************************************************************************
* Tells the function how many places in the stack, data is located at.
*/

int data_location(bool param)
{
	if(param == true)
		param_offset++;

	return data_offset++;
}

/*******************************************************************************
* Generates a location of the current expression going to be executed;
* Used as a place holder to perform a GOTO(Jump) statement
*/

S_Value gen_label()
{
	return GenerateValue(false, INT_T, code_offset);
}

/*******************************************************************************
* Gets the current code stack offset for when a conditional jump to next 
* statement is executed
*/

int reserve_loc()
{
	return code_offset++;
}

/*******************************************************************************
* Allows for modifing stack operations at certain locations. Usefull for filling 
* conditional statements
*/

void gen_code_loc(int addr, int code_op, S_Value arg)
{
	global_code[addr].op  = code_op;
	global_code[addr].arg = arg;
}

/*******************************************************************************
* Add's operations to the function's stack for execution
*/

void gen_code(int code_op, S_Value arg)
{
	gen_code_loc(code_offset++, code_op, arg);
}

/*******************************************************************************
* Changes the offset value where data is going to be held on the stack.
* Then copies the instruction code to the function being executed.
*/

void gen_code_build_stack()
{
	int index 		   = 0;
	S_Function *target = TargetFunction(NULL);

	target->code_offset  = code_offset;
	target->data_offset  = data_offset;
	target->param_offset = param_offset;

	/* Regenerate argument data to reflect all data implementations */
	
	gen_code_loc(0, DATA, GenerateValue(false, INT_T, data_offset-1));

	memcpy(&target->code, &global_code, sizeof(Instruction)*MAXCODE);
}