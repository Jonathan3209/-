#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructures.h"


/*prototypes*/
char* removeSpaces(char *line);
void onlyDest(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC);
int checkType(char *operand);
void buildWord(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC);
void whichTypeToConvert(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC);
void finalizeWord(Word *node, FILE * fpw, int *IC);
void getCommandCode(char *command, char *temp);
void addTypeZero(Word *node, char *operand);
void addTypeOne(Word *node, char *operand);
void addTypeTwo(Word *node, char *operand, FILE * fpw, int *IC, int *DC);
void addTypeThree(Word *node, char *operand, int sourceOrDestiny);
void addTypeThreeForBothOperhands(Word *node, Command *cmd);
void printTheWord(Word *word, FILE * fpw, int *IC);
char* decToBinary(int num, int size, char *finalString);
int checkSizeOfBinaryNumber(int num);
char* decToTwosComplement(char *binary, int size);
void convertWordToTypeFour(char *word, char * string);


void checkCmdType(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC) /*identify the command type*/
{

	if (strcmp(cmd->destiny, "0") == 0)/*checks if there is only one operand*/
	{
		onlyDest(cmd, node, fpw, IC, DC);
	}
	else
	{
		strcpy(cmd->command, removeSpaces(cmd->command));
		strcpy(cmd->source, removeSpaces(cmd->source));
		strcpy(cmd->destiny, removeSpaces(cmd->destiny));

		cmd->sourceType = checkType(cmd->source);
		cmd->destinyType = checkType(cmd->destiny);

		buildWord(cmd, node, fpw, IC, DC);
	}
}

void onlyDest(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC) /*only one operand. convert it into code and save it*/
{
	char string[15] = "0000", *finalString, temp[5];
	strcpy(cmd->destiny, cmd->source);
	cmd->sourceType = 4; /*not adding another word for 4*/
	cmd->destinyType = checkType(cmd->destiny);

	strcpy(cmd->command, removeSpaces(cmd->command));

	strcpy(cmd->source, removeSpaces(cmd->source));
	strcpy(cmd->destiny, removeSpaces(cmd->destiny));

	finalString = (char*)malloc(2 * sizeof(char) + 1);
	getCommandCode(cmd->command, temp);
	if (strcmp(temp, "0") != 0)
	{
		strcat(string, temp);
		strcat(string, decToBinary(cmd->sourceType, 2, finalString));
		strcat(string, decToBinary(cmd->destinyType, 2, finalString));
		strcat(string, "00");
		strcpy(node->commandWord, string);
		finalizeWord(node, fpw, IC);

		free(finalString);
		if (cmd->destinyType != 4)
			whichTypeToConvert(cmd, node, fpw, IC, DC);

	}
}

int checkType(char *operand)
{
	if (operand[0] == '0')
		return 4; /* 4 for nothing*/
	if (strstr(operand, "#") != '\0') /*checks if the command is command type zero - "mov #-1,r1"*/
		return 0;
	else if (strstr(operand, "[") != '\0') /*checks if the command is command type two - "LABEL[8]"*/
		return 2;
	else {
		if (operand[0] == 'r' && (operand[1] >= '1' && operand[1] <= '7')) /*checks if the command is command type three - Register*/
			return 3;
		else return 1; /*command type one - "dec LABLE"*/
	}
}

void buildWord(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC)/*build command word and save if*/
{
	char string[15] = "0000", *finalString, temp[5];
	finalString = (char*)malloc(2 * sizeof(char) + 1);
	getCommandCode(cmd->command, temp);
	if (strcmp(temp, "0") != 0)
	{
		strcat(string, temp);
		strcat(string, decToBinary(cmd->sourceType, 2, finalString));
		strcat(string, decToBinary(cmd->destinyType, 2, finalString));
		strcat(string, "00");
		strcpy(node->commandWord, string);
		finalizeWord(node, fpw, IC);

		whichTypeToConvert(cmd, node, fpw, IC, DC); /* check operands type*/
	}
	free(finalString);
}

void getCommandCode(char *command, char *temp) /* get the command code from file*/
{
	FILE * fp;
	char tempCommand[5], code[5], tempCode[5];
	strcpy(temp , "0");
	fp = fopen("opcodes.txt", "rb");
	if (fp == '\0') printf("error opening file");
	
	while (fscanf(fp, "%s %s", tempCommand, code) != EOF)
	{
		if (strcmp(tempCommand, command) == 0)
		{
			strcpy(temp, code);
		}
	}
	fclose(fp);
}


void whichTypeToConvert(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC) /* checks operands type*/
{
	if (cmd->sourceType == 0)
	{
		addTypeZero(node, cmd->source);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->sourceType == 1)
	{
		addTypeOne(node, cmd->source);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->sourceType == 2)
	{
		addTypeTwo(node, cmd->source, fpw, IC, DC);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->sourceType == 3)
	{
		if (cmd->destinyType == 3)
		{
			addTypeThreeForBothOperhands(node, cmd);
			finalizeWord(node, fpw, IC);
			cmd->destinyType = 4; /* change the destiny type for not adding another word*/
		}
		else
		{
			addTypeThree(node, cmd->source, 1); /*1 for source, 2 for destiny*/
			finalizeWord(node, fpw, IC);
		}
	}
	if (cmd->destinyType == 0)
	{
		addTypeZero(node, cmd->destiny);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->destinyType == 1)
	{
		addTypeOne(node, cmd->destiny);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->destinyType == 2)
	{
		addTypeTwo(node, cmd->destiny, fpw, IC, DC);
		finalizeWord(node, fpw, IC);
	}
	else if (cmd->destinyType == 3)
	{
		addTypeThree(node, cmd->destiny, 2);
		finalizeWord(node, fpw, IC);
	}
}

void finalizeWord(Word *node, FILE * fpw, int *IC) /*finalizing words*/
{
	printTheWord(node, fpw, IC);
	node->next = (Word*)malloc(sizeof(Word));
	node = node->next;
	*IC = *IC + 1;
}


void addTypeZero(Word *node, char *operand) /*handle type zero operand*/
{
	char *token, temp[15], *finalString;
	int num;
	STObj* headSignTblTmp = headSignTbl;
	finalString = (char*)malloc(12 * sizeof(char) + 1);
	token = strtok(operand, "#");
	num = atoi(token);
	if (num == 0)
	{
		while (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, token) != 0)
			headSignTblTmp = headSignTblTmp->next;
		if (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, token) == 0)
		{
			num = headSignTblTmp->addressOrValue;
		}
	}
	strcpy(temp, decToBinary(abs(num), 12, finalString));
	if (num < 0)
		strcpy(temp, decToTwosComplement(temp, 12));
	strcat(temp, "00");
	strcpy(node->commandWord, temp);
	free(finalString);
}

void addTypeOne(Word *node, char *operand) /*handle type one operand*/
{
	char temp[15], *finalString;
	int num, isItExtern = 0;
	STObj* headSignTblTmp = headSignTbl;
	finalString = (char*)malloc(12 * sizeof(char) + 1);

	while (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, operand) != 0)
		headSignTblTmp = headSignTblTmp->next;
	if (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, operand) == 0)
	{
		num = headSignTblTmp->addressOrValue;
		isItExtern = headSignTblTmp->external;
	}
	strcpy(temp, decToBinary(abs(num), 12, finalString));
	if (isItExtern == 1)
		strcat(temp, "01");
	else
		strcat(temp, "10");
	strcpy(node->commandWord, temp);
	free(finalString);
}

void addTypeTwo(Word *node, char *operand, FILE * fpw, int *IC, int* DC) /*handle type two operand*/
{
	char *token, temp[15], address[20], index[20], *finalString;
	int num, isItExtern = 0, indexNumber;
	STObj* headSignTblTmp = headSignTbl;
	finalString = (char*)malloc(12 * sizeof(char) + 1);

	token = strtok(operand, "[");
	strcpy(address, token);
	token = strtok('\0', "[");
	strcpy(index, token);
	token = strtok(index, "]");
	strcpy(index, token);
	indexNumber = atoi(index);

	while (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, address) != 0)
		headSignTblTmp = headSignTblTmp->next;
	if (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, address) == 0)
	{
		num = headSignTblTmp->addressOrValue + 100;
		isItExtern = headSignTblTmp->external;
	}
	strcpy(temp, decToBinary(abs(num), 12, finalString));
	if (isItExtern == 1)
		strcat(temp, "01");
	else
		strcat(temp, "10");
	strcpy(node->commandWord, temp);
	finalizeWord(node, fpw, IC);

	headSignTblTmp = headSignTbl;
	if (indexNumber == 0)
	{
		while (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, index) != 0)
			headSignTblTmp = headSignTblTmp->next;
		if (headSignTblTmp != '\0' && strcmp(headSignTblTmp->name, index) == 0)
		{
			num = headSignTblTmp->addressOrValue;
		}
	}
	else
		num = indexNumber;
	strcpy(temp, decToBinary(abs(num), 12, finalString));
	strcat(temp, "00");
	strcpy(node->commandWord, temp);
	free(finalString);
}
void addTypeThree(Word *node, char *operand, int sourceOrDestiny) /*handle type three operand*/
{
	char *token, *finalString;
	int num;
	finalString = (char*)malloc(3 * sizeof(char) + 1);
	strcpy(node->commandWord, "000000");
	token = strtok(operand, "r");
	num = atoi(token);
	if (sourceOrDestiny == 1) /*1 for source, 2 for destiny*/
	{
		strcat(node->commandWord, decToBinary(num, 3, finalString));
		strcat(node->commandWord, "000");
	}
	else
	{
		strcat(node->commandWord, "000");
		strcat(node->commandWord, decToBinary(num, 3, finalString));
	}
	strcat(node->commandWord, "00");
	free(finalString);
}
void addTypeThreeForBothOperhands(Word *node, Command *cmd) /*handle type three for both operands*/
{
	char *token, srcString[4], destString[4], *finalString;
	int src, dest;
	finalString = (char*)malloc(3 * sizeof(char) + 1);
	strcpy(node->commandWord, "000000");
	token = strtok(cmd->source, "r");
	src = atoi(token);
	token = strtok(cmd->destiny, "r");
	dest = atoi(token);
	strcpy(srcString, decToBinary(src, 3, finalString));
	strcpy(destString, decToBinary(dest, 3, finalString));


	strcat(node->commandWord, srcString);
	strcat(node->commandWord, destString);
	strcat(node->commandWord, "00");
	free(finalString);
}

void printTheWord(Word *word, FILE * fpw, int *IC)
{
	char string[8];

	convertWordToTypeFour(word->commandWord, string);
	fprintf(fpw, "%d %s\n", *IC, string);
}

void convertWordToTypeFour(char *word, char * string) /*convert word from binary to type four*/
{
	int n = 1, i = 0, j = 0;
	char bin[3];
	for (i = 2 * n - 1; i < 14; j++)
	{
		bin[0] = word[i - 1];
		bin[1] = word[i];
		bin[2] = '\0';

		if (strcmp(bin, "00") == '\0')
			string[j] = '*';
		else if (strcmp(bin, "01") == '\0')
			string[j] = '#';
		else if (strcmp(bin, "10") == '\0')
			string[j] = '%';
		else if (strcmp(bin, "11") == '\0')
			string[j] = '!';

		n++;
		i = 2 * n - 1;
	}
	string[7] = '\0';
}