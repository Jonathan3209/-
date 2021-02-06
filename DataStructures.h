/*
DATA STRUCTURES
*/

#include <stdio.h>

typedef struct Words {
	char commandWord[15];
	struct Word *next;
}Word;

typedef struct Commands {
	char command[5];
	char source[20];
	char destiny[20];
	int sourceType;
	int destinyType;
}Command;

typedef struct SignsTableObj {
	char name[20];
	int addressOrValue;
	int typeOfSign; /* 1 for instruction, 2 for command, 3 for macro*/
	int external; /* 1 for external, 0 for not external*/
	struct STObj *next;
}STObj;

typedef struct DataTableObj {
	char data[15];
	int address;
	struct DTObj *next;
}DTObj;

typedef struct Errors {
	char *error;
	struct Errors *next;
}Error;

STObj* headSignTbl;
DTObj* headDataTbl;
Error* headError;
