#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructures.h"

/*prototypes*/
char* removeComment(char *line);
void checkTypeOfSign(char *line, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram);
void checkIfInstructionOrMacro(char *line, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram);
void checkIfLabelOrCommand(char *line, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int* flagFirstSign, int* flagFirstData);
void handleTabs(char *line);
void goToInstructionLine(char *line, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram);
void goToDataInstruction(char *label, char *stringOfData, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram);
void assistToDataInstruction(char *token, char *finalString, char temp[15], int num, DTObj **dataTbl, int *DC, int* flagFirstData, int sizeOfProgram);
void goToStringInstruction(char *label, char *stringOfData, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram);
void goToExternInstruction(char *label, char *stringOfData, STObj **signTbl, int* flagFirstSign);
void GoToLabelWithCommandLine(char *line, STObj **signTbl, int *IC, int* flagFirstSign);
int howMuchWordsItsNeed(char *line);
void goToMacro(char *line, STObj **signTbl, int* flagFirstSign);
char* decToBinary(int num, int size, char *finalString);
char* decToTwosComplement(char *binary, int size);
STObj** signTblMemoryAllocated(STObj** signTbl, int* flagFirstSign);
DTObj** dataTblMemoryAllocated(DTObj** dataTbl, int* flagFirstData);



int firstTravel(FILE *fpProg, FILE *fpProgTemp, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int sizeOfProgram)
{
	char line[50], *tokenLine;
	int *flagFirstSign, *flagFirstData, flagComment = 0;
	*IC = 100;
	flagFirstSign = (int*)calloc(1, sizeof(int));
	flagFirstData = (int*)calloc(1, sizeof(int));
	*flagFirstSign = 1;
	*flagFirstData = 1;
	while (fgets(line, sizeof(line), fpProgTemp))
	{
		tokenLine = strtok(line, "\n");
		if (line[0] == ';')
			flagComment = 1;
		else if (strstr(line, ";") != '\0')
			strcpy(line, removeComment(line));
		if (flagComment != 1 && tokenLine != '\0') /*do nothing if the line is only comment or the line is empty*/
		{
			sizeOfProgram += howMuchWordsItsNeed(line);
		}
		flagComment = 0;
	}
	while (fgets(line, sizeof(line), fpProg))
	{
		tokenLine = strtok(line, "\n");
		if (line[0] == ';')
			flagComment = 1;
		else if (strstr(line, ";") != '\0')
			strcpy(line, removeComment(line));
		if (flagComment != 1 && tokenLine != '\0') /*do nothing if the line is only comment or the line is empty*/
		{
			checkTypeOfSign(tokenLine, signTbl, dataTbl, IC, DC, flagFirstSign, flagFirstData, sizeOfProgram);
		}
		flagComment = 0;
	}
	return sizeOfProgram;
}

void checkTypeOfSign(char *line, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram)
{
	if (strstr(line, ".") != '\0') {
		checkIfInstructionOrMacro(line, signTbl, dataTbl, DC, flagFirstSign, flagFirstData, sizeOfProgram); /*then check if it's intruction or macro*/
	}
	else if (strstr(line, ":") != '\0')
		checkIfLabelOrCommand(line, signTbl, dataTbl, IC, DC, flagFirstSign, flagFirstData); /*then check if it's label for command or label for instruction*/
	else
		*IC += howMuchWordsItsNeed(line);
	/*else do nothing in first travel*/
}

void checkIfInstructionOrMacro(char *line, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram)
{
	if (strstr(line, "\t") != '\0')
		handleTabs(line);
	if (strstr(line, ".define") != '\0')
		goToMacro(line, signTbl, flagFirstSign);
	else
		goToInstructionLine(line, signTbl, dataTbl, DC, flagFirstSign, flagFirstData, sizeOfProgram);
}

void checkIfLabelOrCommand(char *line, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int* flagFirstSign, int* flagFirstData)
{
	if (strstr(line, ":") != '\0')
		GoToLabelWithCommandLine(line, signTbl, IC, flagFirstSign);
	/*else: if it's only a command line without label, then do nothing in first travel*/
}

/*handling instruction line:*/
void goToInstructionLine(char *line, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram)
{
	/*cleaning and spliting the line*/
	char label[25], *token, instructionType[8], stringOfData[14];
	if (strstr(line, ":") != '\0')
	{
		token = strtok(line, ":");
		strcpy(label, token);
		token = strtok('\0', ":");
		strcpy(line, token);
	}
	token = strtok(line, " ");
	strcpy(instructionType, token);
	token = strtok('\0', " ");
	strcpy(stringOfData, token);
	if (strcmp(instructionType, ".data") == 0)
	{
		while (token != '\0')
		{
			token = strtok('\0', " ");
			if (token != '\0')
				strcat(stringOfData, token);
		}
	}
	if (strstr(stringOfData, "\"") != '\0')
	{
		token = strtok(stringOfData, "\"");
		strcpy(stringOfData, token);
	}
	/*end of cleaning and spliting*/

	if (strcmp(instructionType, ".data") == 0)
		goToDataInstruction(label, stringOfData, signTbl, dataTbl, DC, flagFirstSign, flagFirstData, sizeOfProgram);
	else if (strcmp(instructionType, ".string") == 0)
		goToStringInstruction(label, stringOfData, signTbl, dataTbl, DC, flagFirstSign, flagFirstData, sizeOfProgram);
	else if (strcmp(instructionType, ".extern") == 0)
		goToExternInstruction(label, stringOfData, signTbl, flagFirstSign);
}
void goToDataInstruction(char *label, char *stringOfData, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram)
{
	char *token, *finalString, temp[15];
	int num = 0;
	finalString = (char*)malloc(14 * sizeof(char) + 1);
	token = (char*)malloc(14 * sizeof(char) + 1);
	*signTbl = signTblMemoryAllocated(signTbl, flagFirstSign);


	strcpy((*signTbl)->name, label);
	(*signTbl)->addressOrValue = *DC + sizeOfProgram;
	(*signTbl)->typeOfSign = 1;
	(*signTbl)->external = 0;

	if (strstr(stringOfData, ",") != '\0')
	{
		token = strtok(stringOfData, ",");
		while (token != '\0')
		{
			assistToDataInstruction(token, finalString, temp, num, dataTbl, DC, flagFirstData, sizeOfProgram);
			token = strtok('\0', ",");
		}
	}
	else
	{
		assistToDataInstruction(stringOfData, finalString, temp, num, dataTbl, DC, flagFirstData, sizeOfProgram);
	}
}
void assistToDataInstruction(char *token, char *finalString, char temp[15], int num, DTObj **dataTbl, int *DC, int* flagFirstData, int sizeOfProgram)
{
	int flag = 0;
	num = atoi(token);
	if (num == 0)
	{
		STObj* headSignTblTmp = headSignTbl;
		while (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, token) != 0)
			headSignTblTmp = headSignTblTmp->next;
		if (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, token) == 0)
		{
			num = headSignTblTmp->addressOrValue;
			flag = 1; /*if changed in case of num == 0*/
		}
	}
	if (num < 0)
		strcpy(temp, decToTwosComplement(decToBinary(abs(num), 14, finalString), 14));
	else if (num > 0)
		strcpy(temp, decToBinary(abs(num), 14, finalString));
	else if (flag == 1)
		strcpy(temp, decToBinary(num, 14, finalString));

	*dataTbl = dataTblMemoryAllocated(dataTbl, flagFirstData);
	strcpy((*dataTbl)->data, temp);
	(*dataTbl)->address = *DC + sizeOfProgram;
	*DC += 1;
}
void goToStringInstruction(char *label, char *stringOfData, STObj **signTbl, DTObj **dataTbl, int *DC, int* flagFirstSign, int* flagFirstData, int sizeOfProgram)
{
	int i;
	*signTbl = signTblMemoryAllocated(signTbl, flagFirstSign);
	*dataTbl = dataTblMemoryAllocated(dataTbl, flagFirstData);


	strcpy((*signTbl)->name, label);
	(*signTbl)->addressOrValue = *DC + sizeOfProgram;
	(*signTbl)->typeOfSign = 1;
	(*signTbl)->external = 0;

	for (i = 0; i < strlen(stringOfData); i++)
	{
		(*dataTbl)->data[0] = stringOfData[i];
		(*dataTbl)->data[1] = '\0';
		(*dataTbl)->address = *DC + sizeOfProgram;
		*DC += 1;
		if (i < strlen(stringOfData))
		{
			(*dataTbl)->next = (DTObj*)malloc(sizeof(DTObj));
			*dataTbl = (*dataTbl)->next;
			(*dataTbl)->next = '\0';
		}
	}

	/*(*dataTbl)->data[0] = '0';*/
	(*dataTbl)->data[0] = '\0';
	(*dataTbl)->address = *DC + sizeOfProgram;
	*DC += 1;
}
void goToExternInstruction(char *label, char *stringOfData, STObj **signTbl, int* flagFirstSign)
{
	*signTbl = signTblMemoryAllocated(signTbl, flagFirstSign);

	if (label != '\0')
		strcpy((*signTbl)->name, stringOfData);
	(*signTbl)->addressOrValue = 0;
	(*signTbl)->typeOfSign = 0;
	(*signTbl)->external = 1;
}
/*end of handling instruction line:*/

/*handling Command line:*/
void GoToLabelWithCommandLine(char *line, STObj **signTbl, int *IC, int* flagFirstSign)
{
	char label[25], *token;
	*signTbl = signTblMemoryAllocated(signTbl, flagFirstSign);

	token = strtok(line, ":");
	strcpy(label, token);
	token = strtok('\0', ":");
	strcpy(line, token);

	strcpy((*signTbl)->name, label); /*נופל כאן*/
	(*signTbl)->addressOrValue = *IC;
	*IC += howMuchWordsItsNeed(line);
	(*signTbl)->typeOfSign = 2;
	(*signTbl)->external = 0;

}

int howMuchWordsItsNeed(char *line)
{
	char *token, src[20], dest[20];
	int i = 0, flagForTypeThree = 0;
	if ((strstr(line, ".") == '\0'))
	{
		if (strstr(line, ",") == '\0') /*checks if there is only one argument*/
		{
			do
			{
				token = strtok(line, " ");
				token = strtok('\0', " ");
				i++;
			} while (token != '\0');
		}
		else
		{
			i++; /*for command*/
			token = strtok(line, ",");
			strcpy(src, token);
			token = strtok('\0', ",");
			strcpy(dest, token);

			token = strtok(src, " ");
			token = strtok('\0', " ");
			strcpy(src, token);

			if (strstr(src, "#") != '\0')
				i++;
			else if (strstr(src, "[") != '\0')
				i += 2;
			else if (src[0] == 'r' && (src[1] >= '1' && src[1] <= '7'))
			{
				flagForTypeThree = 1;
				i++;
			}
			else
				i++;

			if (strstr(dest, "#") != '\0')
				i++;
			else if (strstr(dest, "[") != '\0')
				i += 2;
			else if (src[0] == 'r' && (src[1] >= '1' && src[1] <= '7') && flagForTypeThree != 1)
				i++;
			else if (flagForTypeThree != 1)
				i++;
		}

	}
	return i;
}
/*end of Command line:*/

/*handling macro*/
void goToMacro(char *line, STObj **signTbl, int* flagFirstSign) /*fills the signTbl*/
{
	char *token, temp[20];
	*signTbl = signTblMemoryAllocated(signTbl, flagFirstSign);

	token = strtok(line, " ");
	token = strtok('\0', " ");
	/*strcpy(temp, token);*/
	strcpy((*signTbl)->name, token);
	token = strtok('\0', " ");
	token = strtok('\0', " ");
	(*signTbl)->addressOrValue = (atoi(token));
	(*signTbl)->typeOfSign = 3;
	(*signTbl)->external = 0;
}
/*end of handling macro*/


STObj** signTblMemoryAllocated(STObj** signTbl, int* flagFirstSign)
{
	if (*signTbl == '\0')
	{
		*signTbl = (STObj*)malloc(sizeof(STObj));
		(*signTbl)->next = '\0';
	}
	if (*signTbl != '\0' && *flagFirstSign != 1)
	{
		(*signTbl)->next = (STObj*)malloc(sizeof(STObj));
		*signTbl = (*signTbl)->next;
		(*signTbl)->next = '\0';
	}
	*flagFirstSign = 0;
	return *signTbl; /*changed from *signTbl*/
}
DTObj** dataTblMemoryAllocated(DTObj** dataTbl, int* flagFirstData)
{
	if (*dataTbl == '\0')
	{
		*dataTbl = (STObj*)malloc(sizeof(STObj));
		(*dataTbl)->next = '\0';
	}
	if (*dataTbl != '\0' && *flagFirstData != 1)
	{
		(*dataTbl)->next = (STObj*)malloc(sizeof(STObj));
		*dataTbl = (*dataTbl)->next;
		(*dataTbl)->next = '\0';
	}
	*flagFirstData = 0;
	return *dataTbl;
}
