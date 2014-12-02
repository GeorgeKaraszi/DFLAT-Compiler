#ifndef VALUEHANDLER_H_
#define VALUEHANDLER_H_

typedef int bool;
#define true  1
#define false 0

typedef struct _S_VALUE_
{
	int valtype;  /* Value type (Read definitions below) */
	
	union {

  	int ival;
  	double fval;
  	char *sval;
  	bool bval;

  }x;				/*Ambigous name for ease of access */

} S_Value;

/* Value type definitions */
#define UNKNOWN 0
#define INT_T   1
#define FLOAT_T 2
#define STR_T   3
#define BOOL_T  4




/* Printing functions */
void PrintValue(S_Value a);

S_Value GenerateValue(bool SVal, int type, ...);

/*Math type functions */
S_Value Add_S(S_Value a, S_Value b);
S_Value Sub_S(S_Value a, S_Value b);
S_Value Mult_S(S_Value a, S_Value b);
S_Value Div_S(S_Value a, S_Value b);
S_Value Mod_S(S_Value a, S_Value b);
S_Value PowerRise_S(S_Value a, S_Value b);
S_Value CompareValues_S(S_Value a, S_Value b, int cOP);
S_Value NotExp_S(S_Value a);

#endif