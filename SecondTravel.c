#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructures.h"

char* removeComment(char *line);
char* removeSpacesInSource(char *line);
void makeEntryFile(char *label, FILE *fpwEntry);
void makeExternFile(FILE *fpwExtern);
void printDataTable(FILE *fpw, int *IC);
void checkCmdType(Command *cmd, Word *node, FILE * fpw, int *IC, int *DC);
int checkIfBelongsToSecondTravel(char *line);
int splitLabelAndCommand(char *line);
void buildCmd(Command *cmd, char *line);
void handleTabs(char *line);
void finalizeWord(Word *node, FILE * fpw, int *IC);
char* decToBinary(int num, int size, char *finalString);

void convert(Word *node, FILE *fpProg, FILE *fpw, FILE *fpwEntry, FILE *fpwExtern, int *IC, int *DC)
{
	char line[50], *tokenLine, flagComment = 0, *token;
	Command *cmd;
	STObj* headSignTblTmp = headSignTbl;
	cmd = (Command*)malloc(sizeof(Command));

	while (fgets(line, sizeof(line), fpProg))
	{
		/*handle \n*/
		tokenLine = strtok(line, "\n");
		/*handle comments*/
		if (line[0] == ';')
			flagComment = 1;
		else if (strstr(line, ";") != '\0')
			strcpy(line, removeComment(line));
		if (flagComment != 1 && tokenLine != '\0') /*do nothing if the line is only comment or the line is empty*/
		{
			/*handle \t (tabs)*/
			if (strstr(line, "\t") != '\0')
				handleTabs(line);
			if (checkIfBelongsToSecondTravel(line) == 1) /* checks if belongs to second travel*/
			{
				/* convert to codes*/
				buildCmd(cmd, line);
				checkCmdType(cmd, node, fpw, IC, DC);
			}
			else if (strstr(line, ".entry") != '\0' && fpwEntry != '\0') /*make entry file*/
			{
				token = strtok(line, " ");
				token = strtok('\0', " ");
				makeEntryFile(token, fpwEntry);
			}
		}
		flagComment = 0;
	}

	/*print data table*/
	printDataTable(fpw, IC);
	/*make extern file*/
	if (fpwExtern != '\0')
		makeExternFile(fpwExtern);
}

int checkIfEntryFile(char *fileName) /*checks if there is need for entry file*/
{
	char line[50];
	FILE *newFpProg;
	newFpProg = fopen(fileName, "r");
	if (newFpProg == '\0') printf("error opening file");

	while (fgets(line, sizeof(line), newFpProg))
	{
		if (strstr(line, ".entry") != '\0')
		{
			fclose(newFpProg);
			return 1;
		}
	}
	fclose(newFpProg);
	return 0;
}
int checkIfExternFile(char *fileName) /*checks if there is need for extern file*/
{
	char line[50];
	FILE *newFpProg;
	newFpProg = fopen(fileName, "r");
	if (newFpProg == '\0') printf("error opening file");

	while (fgets(line, sizeof(line), newFpProg))
	{
		if (strstr(line, ".extern") != '\0')
		{
			fclose(newFpProg);
			return 1;
		}
	}
	fclose(newFpProg);
	return 0;
}

void makeEntryFile(char *label, FILE *fpwEntry) /*make entry file*/
{
	STObj *signTemp = headSignTbl;
	while (signTemp != '\0' && strcmp(signTemp->name, label) != 0)
	{
		signTemp = signTemp->next;
	}
	if (signTemp != '\0' && strcmp(signTemp->name, label) == 0)
	{
		fprintf(fpwEntry, "%s %d\n", signTemp->name, signTemp->addressOrValue);

	}
}
void makeExternFile(FILE *fpwExtern) /*make extern file*/
{
	STObj *signTemp = headSignTbl;
	while (signTemp != '\0') /*run through sign table*/
	{
		if (signTemp->external == 1)
		{
			fprintf(fpwExtern, "%s %d\n", signTemp->name, signTemp->addressOrValue);
		}
		signTemp = signTemp->next;
	}
}


void printDataTable(FILE *fpw, int *IC) /* print the data table*/
{
	DTObj *dataTemp = headDataTbl;
	char *finalString;
	int number = 0;
	Word *wordTemp = '\0';
	while (dataTemp != '\0') /* run through data table*/
	{
		wordTemp = (Word*)malloc(sizeof(Word));
		finalString = (char*)malloc(14 * sizeof(char) + 1);
		if (strlen(dataTemp->data) == 1)
		{
			number = dataTemp->data[0];
			strcpy(wordTemp->commandWord, decToBinary(number, 14, finalString));
			finalizeWord(wordTemp, fpw, IC);
		}
		else if (strlen(dataTemp->data) == 0)
		{
			strcpy(wordTemp->commandWord, decToBinary(0, 14, finalString));
			finalizeWord(wordTemp, fpw, IC);
		}
		else
		{
			strcpy(wordTemp->commandWord, dataTemp->data);
			finalizeWord(wordTemp, fpw, IC);
		}
		dataTemp = dataTemp->next;
	}
}

int checkIfBelongsToSecondTravel(char *line)
{
	if (strstr(line, ".") != '\0')
		return 0;
	else if (strstr(line, ":") != '\0')
		return splitLabelAndCommand(line);
	else
		return 1; /*no instruction, no macro*/
}
int splitLabelAndCommand(char *line) /*split label and comment*/
{
	char *token;
	if (strstr(line, ":") != '\0')
	{
		token = strtok(line, ":");
		token = strtok('\0', ":");
		strcpy(line, token);
	}
	return 1;
}

void buildCmd(Command *cmd, char *line)
{
	char *token;
	strcpy(line, removeSpacesInSource(line));
	token = strtok(line, " ");
	strcpy(cmd->command, token);	
	token = strtok('\0', " ");
	if (token != '\0')
	{
		strcpy(cmd->source, token);
		token = strtok('\0', " ");
		if(token != '\0')
			strcpy(cmd->destiny, token);
		if (strstr(cmd->source, ",") != '\0')
		{
			token = strtok(cmd->source, ",");
			strcpy(cmd->source, token);
			token = strtok('\0', ",");
			if (token != '\0')
				strcpy(cmd->destiny, token);
		}
		else
			strcpy(cmd->destiny, "0");
	}
	else
	{
		strcpy(cmd->source, "0");
		strcpy(cmd->destiny, "0");
	}
}
