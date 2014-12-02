#include "dflat.h"
#include "valueHandler.h"


/******************************************************************************
* Prints the value when ever a HORN function is called
*/

void PrintValue(S_Value a)
{

	switch(a.valtype)
	{
		case STR_T:
			printf("%s\n", a.x.sval);
			break;
		case FLOAT_T:
			printf("%g\n", a.x.fval);
			break;
		case INT_T:
			printf("%d\n", a.x.ival);
			break;
		case BOOL_T:
			printf("%s\n", a.x.bval == true ? "TRUE":"FALSE");
			break;
		default:
			/*yyerror("Unknown Value\n");*/
			printf("UNKNOWN Value\n");
			break;
	}
}


/*******************************************************************************
* Generates a S_VALUE structure with the given type and value provided
*/

S_Value GenerateValue(bool SVAL, int type, ...)
{
	va_list argument;   /* Arguemnt list of the function */
	S_Value gen;


	va_start(argument, type);
	if(SVAL == true)
		gen = va_arg(argument, S_Value);
	else
	{
		switch(type)
		{
			case INT_T:
				gen.valtype = INT_T;
				gen.x.ival = va_arg(argument, int);
			break;
			case FLOAT_T:
				gen.valtype = FLOAT_T;
				gen.x.fval  = va_arg(argument, double);
			break;
			case STR_T:
				gen.valtype = STR_T;
				gen.x.sval  = strdup(va_arg(argument, char*));
			break;
			case BOOL_T:
				gen.valtype = BOOL_T;
				gen.x.bval  = va_arg(argument, bool);
			break;
			default:
				gen.valtype = UNKNOWN;
			break;
		}
	}
	va_end(argument);

	return gen;
}

/******************************************************************************
* Add's varaibles together depending on their type
*/

S_Value Add_S(S_Value a, S_Value b)
{
	S_Value result;

	if(a.valtype == STR_T && b.valtype == STR_T)
	{
		result.x.sval = strcat(a.x.sval, b.x.sval);
		result.valtype = STR_T;
	}
	else if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.fval + b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = a.x.fval + b.x.ival;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.ival+ b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.ival= a.x.ival+ b.x.ival;
		result.valtype = INT_T;
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************
* Subtracks varaibles together depending on their type
*/

S_Value Sub_S(S_Value a, S_Value b)
{
	S_Value result;

	if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.fval - b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = a.x.fval - b.x.ival;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.ival- b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.ival= a.x.ival- b.x.ival;
		result.valtype = INT_T;
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************
* Multiplies varaibles together depending on their type
*/

S_Value Mult_S(S_Value a, S_Value b)
{
	S_Value result;

	if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.fval * b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = a.x.fval * b.x.ival;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.ival* b.x.fval;
		result.valtype = FLOAT_T;
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.ival= a.x.ival* b.x.ival;
		result.valtype = INT_T;
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************-
*Divides varaibles together depending on their type. 
*/

S_Value Div_S(S_Value a, S_Value b)
{
	S_Value result;

	result.valtype = FLOAT_T;

	if(b.valtype == FLOAT_T)
	{
		if(b.x.fval == 0)
		{
			yyerror("Cannot Divide by zero!");
			result.x.fval = FLT_MAX;
			return result;
		}
	}
	else if(b.x.ival== 0)
	{
		yyerror("Cannot Divide by zero!");
		result.x.fval = FLT_MAX;
		return result;
	}


	if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.fval / b.x.fval;
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = a.x.fval / b.x.ival;
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = a.x.ival/ b.x.fval;
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.ival= a.x.ival/ b.x.ival;
		result.valtype = INT_T;
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************
* Modulo's varaibles together depending on their type
*/

S_Value Mod_S(S_Value a, S_Value b)
{
	S_Value result;
	result.valtype = FLOAT_T;

	if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = fmod(a.x.fval, b.x.fval);
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = fmod(a.x.fval,b.x.ival);
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = fmod(a.x.ival, b.x.fval);
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.ival= a.x.ival% b.x.ival;
		result.valtype = INT_T;
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************
* Rises the power of the given varaible
*/

S_Value PowerRise_S(S_Value a, S_Value b)
{
	S_Value result;
	result.valtype = FLOAT_T;

	if(a.valtype == FLOAT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = pow(a.x.fval, b.x.fval);
	}
	else if(a.valtype == FLOAT_T && b.valtype == INT_T)
	{
		result.x.fval = pow(a.x.fval, b.x.ival);
	}
	else if(a.valtype == INT_T && b.valtype == FLOAT_T)
	{
		result.x.fval = pow(a.x.ival, b.x.fval);
	}
	else if(a.valtype == INT_T && b.valtype == INT_T)
	{
		result.x.fval= pow(a.x.ival, b.x.ival);
	}
	else
	{
		yyerror("Unkown datatype");
	}

	return result;
}

/******************************************************************************
* Comapre string values based on the given conditional operator
*/

bool CompareStringValues(S_Value a, S_Value b, int cOP)
{
	enum code_ops compareType = cOP;

	switch(compareType)
	{
		case LT:
			return  strcmp(a.x.sval, b.x.sval) < 0  ?  true:false;
		case LE:
			return  strcmp(a.x.sval, b.x.sval) <= 0 ?  true:false;
		case GT:
			return  strcmp(a.x.sval, b.x.sval) > 0  ?  true:false;
		case GE:
			return  strcmp(a.x.sval, b.x.sval) >= 0 ?  true:false;
		case ET:
			return  strcmp(a.x.sval, b.x.sval) == 0 ?  true:false;
		case NE:
			return  strcmp(a.x.sval, b.x.sval) != 0 ?  true:false;
		default:
			return 0;
	}
}

/******************************************************************************
* Compares condition statments based on the given comparing operator
*/

S_Value CompareValues_S(S_Value a, S_Value b, int cOP)
{
	S_Value result;				        /* Final results of condition */
	enum code_ops compareType    = cOP; /* Comparing operator type */
	double CompareA, CompareB    = 0.0;	/* Comparing variables used */

	result.valtype = BOOL_T;

	/* Both are string values, so compare string appropriately */
	if(a.valtype == STR_T && b.valtype == STR_T) 
	{
		result.x.bval = CompareStringValues(a, b, cOP);
	}
	/* Values are number types */
	else if(a.valtype != STR_T && b.valtype != STR_T)
	{
		switch(a.valtype)
		{
			case INT_T:
				CompareA = a.x.ival;
				break;
			case FLOAT_T:
				CompareA = a.x.fval;
				break;
			case BOOL_T:
				CompareA = a.x.bval;
				break;
		}

		switch(b.valtype)
		{
			case INT_T:
				CompareB = b.x.ival;
				break;
			case FLOAT_T:
				CompareB = b.x.fval;
				break;
			case BOOL_T:
				CompareB = b.x.bval;
				break;
		}

		switch(compareType)
		{
			case LT:
				result.x.bval = CompareA < CompareB ?  true:false;
			break;
			case LE:
				result.x.bval = CompareA <= CompareB ? true:false;
			break;
			case GT:
				result.x.bval = CompareA > CompareB ?  true:false;
			break;
			case GE:
				result.x.bval = CompareA >= CompareB ? true:false;
			break;
			case ET:
				result.x.bval = CompareA == CompareB ? true:false;
			break;
			case NE:
				result.x.bval = CompareA != CompareB ? true:false;
			break;
		}
	}
	/* Unkown value type or combination */
	else
	{
		result.x.bval = false;
	}

	return result;
}

/******************************************************************************
* Compares condition statments based on the given comparing operator
*/
S_Value NotExp_S(S_Value a)
{
	S_Value results;
	results.valtype = BOOL_T;

	switch(a.valtype)
	{
		case STR_T:
			results.x.bval = !a.x.sval ? true:false;
		break;
		case INT_T:
			results.x.bval = !a.x.ival ? true:false;
		break;
		case FLOAT_T:
			results.x.bval = !a.x.fval ? true:false;
		break;
		case BOOL_T:
			results.x.bval = !a.x.bval ? true:false;
		break;
		default:
			results.x.bval = false;
	}

	return results;
}