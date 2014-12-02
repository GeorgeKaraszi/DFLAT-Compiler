/******************************************************************************
*	Most of the hashtable functionaly credit goes to:
*	
*	Author User ID: tonious
*	URL: https://gist.github.com/tonious/1377667#file-hash-c-L11
*
*/

#include "ht_funcmgr.h"

/******************************************************************************
*	Create the hashtable
*/

S_HashTable *CreateHashTable(int size)
{
	int index              = 0;
	S_HashTable *hashTable = NULL;

	if(size < 1)
		return NULL;

	if((hashTable = malloc(sizeof(S_HashTable*))) == NULL)
		return NULL;

	/* Allocate varaiable table array*/
	if((hashTable->S_Func = malloc(sizeof(S_Function*) * size))  == NULL)
		return NULL;

	/* Clear each entry */
	for(index = 0; index < size; index++)
		hashTable->S_Func[index] = NULL;

	hashTable->size = size;

	return hashTable;
}

/******************************************************************************
*	Hash the key value for a position within the table
*/

int HashKey(S_HashTable *hashTable, char *funcName)
{
	unsigned long int hashval = 0;
	int i 					  = 0;

	while(hashval < ULONG_MAX && i < strlen(funcName))
	{
		hashval = hashval << 8;
		hashval += funcName[i];
		i++;
	}

	return hashval % hashTable->size;
}


/******************************************************************************
*	Create a new function entry for the hash table
*/

S_Function *NewFuncEntry(char *funcName)
{
	S_Function *fvar = NULL;

	if((fvar = malloc(sizeof(S_Function))) == NULL)
		return NULL;

	if((fvar->funcName = strdup(funcName)) == NULL)
		return NULL;

	fvar->code_offset = 0;
	fvar->next 		  = NULL;

	return fvar;
}

/******************************************************************************
*	Create a new variable entry for the function table
*/

S_Variable *NewVarEntry(char *varName, int data_offset)
{
	S_Variable *svar = NULL;

	if((svar = malloc(sizeof(S_Variable))) == NULL)
		return NULL;

	if((svar->varName = strdup(varName)) == NULL)
		return NULL;

	svar->data_offset = data_offset;
	svar->next = NULL;

	return svar;
}

/******************************************************************************
*	Insert into hash table
*/

int InsertFunction(S_HashTable *hashTable, char *funcName)
{
	int index 		  = 0;				/* index to hash table */
	S_Function *next  = NULL;			/* Link list entry of hash table */
	S_Function *last  = NULL;			/* Prev Link list entry of hash table */
	S_Function *entry = NewFuncEntry(funcName); /* New entry  */

	index = HashKey(hashTable, funcName);

	next = hashTable->S_Func[index];

	printf("\n\nDEBUG:Inserting %s\n\n", funcName);

	while(next != NULL && strcmp(next->funcName, funcName) > 0)
	{
		last = next;
		next = next->next;
	}

	/* Entry match found. Change value of the variable */
	if(next != NULL && strcmp(next->funcName, funcName) == 0)
	{
		printf("Function (%s) already defined!\n", funcName);
		return -1;
	}
	else /* Entry match not found. Insert into table */
	{
		if(next == hashTable->S_Func[index])/* Beginning of link list */
		{
			entry->next = next;
			hashTable->S_Func[index] = entry;
		}
		else if(next == NULL)		    	/* End of link list */
		{
			last->next = entry;
		}
		else								/* Middle of link list */
		{
			entry->next = next;
			last->next = entry;
		}
	}

	return 0;

}

/******************************************************************************
*	Insert into hash table
*/

int InsertVariable(S_Function *target, char *varName, int data_offset)
{
	S_Variable *next  = NULL;
	S_Variable *prev  = NULL;
	S_Variable *entry = NewVarEntry(varName, data_offset);

	next = target->VarData;

	while(next != NULL && strcmp(next->varName, varName) > 0)
	{
		prev = next;
		next = next->next;
	}

	if(next != NULL && strcmp(next->varName, varName) == 0)
	{
		return -1;
	}
	else /* Entry match not found. Insert into table */
	{
		if(next == target->VarData)/* Beginning of link list */
		{
			entry->next = next;
			target->VarData = entry;
		}
		else if(next == NULL)		    	/* End of link list */
		{
			prev->next = entry;
		}
		else								/* Middle of link list */
		{
			entry->next = next;
			prev->next = entry;
		}
	}

	return 0;

}

/******************************************************************************
*	Get Variable from hash table
*/

S_Variable *FindVariable(S_Function *target, char *varName)
{
	S_Variable *svar = target->VarData;/* Entry from the hash table */


	/* Step through looking for the matching pair */
	while(svar != NULL && strcmp(svar->varName, varName) > 0)
	{
		svar = svar->next;
	}

	if(svar == NULL || strcmp(svar->varName, varName) != 0)
	{
		return NULL;
	}


	return svar;
}

/******************************************************************************
*	Get the value of the variable
*/

S_Value FindValue(S_Function *target, char *varName)
{
	S_Variable *svar = FindVariable(target, varName);
	S_Value errorReturn;

	if(svar == NULL)
	{
		errorReturn.valtype = UNKNOWN;
		return errorReturn;
	}

	return GenerateValue(false, INT_T, svar->data_offset);
}

/******************************************************************************
*	Find the Function stored in the hash table
*/

S_Function *FindFunction(S_HashTable *hashTable, char *funcName)
{
	int index 		  = 0;		/* Index to the hash table*/
	S_Function *entry = NULL;   /* Function entry in the HT */

	index = HashKey(hashTable, funcName);
	entry = hashTable->S_Func[index];

	while(entry != NULL && strcmp(entry->funcName, funcName) > 0)
	{
		entry = entry->next;
	}

	if(entry == NULL || strcmp(entry->funcName, funcName) != 0)
	{
		printf("ERROR:Could not find (%s) function!\n", funcName);
		return NULL;
	}

	return entry;

}

/*******************************************************************************
* Free memory from the varaiable list recurisivly
*/

void DestroyVariableList(S_Variable *VarData)
{
	if(VarData != NULL)
	{
		DestroyVariableList(VarData->next);
		free(VarData->varName);
		free(VarData);
	}
}

/*******************************************************************************
* Free memory the function is holding recursivly
*/


void DestroyFunctionList(S_Function *func)
{
	if(func != NULL)
	{
		DestroyFunctionList(func->next);
		DestroyVariableList(func->VarData);
		free(func->funcName);
		free(func);
	}
}

/******************************************************************************
*	Craws through the hash table freeing memory
*/

void DestroyHashTable(S_HashTable *hashTable)
{
	int index         = 0;
	S_Variable *entry = NULL;
	S_Variable *next  = NULL;

	for(index = 0; index < hashTable->size; index++)
	{
		DestroyFunctionList(hashTable->S_Func[index]);
	}

	free(hashTable->S_Func);
	free(hashTable);
}


/******************************************************************************
*	End of ht_varmgr.c
*/