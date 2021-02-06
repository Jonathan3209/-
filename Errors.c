#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DataStructures.h"

char* removeSpaces(char *line);
void getCommandCode(char *command, char *temp);
char* removeComment(char *line);
int checkIfBelongsToSecondTravel(char *line);
void handleTabs(char *line);
void checkLine(char *line, int *lineNumber, int *errorSizeCounter, Error **errorObj, int *flagFirstError);
void error(int *lineNumber, char *command, Error **errorObj, int *flagFirstError);
Error** errorMemoryAllocated(Error** errorObj, int* flagFirstError);
void checkErrorsWithTwoOperhands(char *source, char *destiny, int *lineNumber, char *command, int *errorSizeCounter, Error **errorObj, int *flagFirstError);
void checkErrorsWithOneOperhand(char *destiny, int *lineNumber, char *command, int *errorSizeCounter, Error **errorObj, int *flagFirstError);

int checkForErrors(FILE *fpProg, Error **errorObj)
{
	char line[50], *tokenLine;
	int *lineNumber, *errorSizeCounter, *flagFirstError;
	int flagComment = 0;
	Error *errorTemp = headError;
	lineNumber = (int*)malloc(sizeof(int));
	errorSizeCounter = (int*)malloc(sizeof(int));
	flagFirstError = (int*)calloc(1, sizeof(int));
	*flagFirstError = 1;
	*lineNumber = 1;
	*errorSizeCounter = 0;
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
			if (checkIfBelongsToSecondTravel(line))
			{
				checkLine(line, lineNumber, errorSizeCounter, errorObj, flagFirstError);
			}
		}
		*lineNumber += 1;
		flagComment = 0;
	}
	if (*errorSizeCounter != 0)
	{
		printf("Errors:\n");
		while (errorTemp != '\0')
		{
			printf("%s\n", errorTemp->error);
			errorTemp = errorTemp->next;
		}
	}
	return *errorSizeCounter;
}

void checkLine(char *line, int *lineNumber, int *errorSizeCounter, Error **errorObj, int *flagFirstError)
{
	char *token, *tokenTwo, *command, *source, *destiny, temp[5];
	token = strtok(line, ",");
	tokenTwo = strtok('\0', " ");

	command = (char*)malloc(strlen(token) * sizeof(char) + 1);
	strcpy(command, token);
	token = strtok(command, " ");
	token = strtok('\0', " ");
	strcpy(command, removeSpaces(command));
	getCommandCode(command, temp);

	if (strcmp(temp, "0") == 0)
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	if (token != '\0')
	{
		source = (char*)malloc(strlen(token) * sizeof(char) + 1);
		strcpy(source, token);
		strcpy(source, removeSpaces(source));

		if (tokenTwo != '\0')
		{
			destiny = (char*)malloc(strlen(tokenTwo) * sizeof(char) + 1);
			strcpy(destiny, tokenTwo);
			strcpy(destiny, removeSpaces(destiny));
			checkErrorsWithTwoOperhands(source, destiny, lineNumber, command, errorSizeCounter, errorObj, flagFirstError);
		}
		else
		{
			destiny = (char*)malloc(strlen(source) * sizeof(char) + 1);
			strcpy(destiny, source);
			strcpy(destiny, removeSpaces(destiny));
			checkErrorsWithOneOperhand(destiny, lineNumber, command, errorSizeCounter, errorObj, flagFirstError);
		}
	}
}

void error(int *lineNumber, char *command, Error **errorObj, int *flagFirstError)
{
	char *temp = "\nError in line ", *continuedMsg = ". Error in: ", c[3], *newMsg;
	newMsg = (char*)malloc((strlen(temp) + strlen(continuedMsg)) * sizeof(char) + 3);
	strcpy(newMsg, temp);
	/*strcat(newMsg, itoa(*lineNumber, c, 10));*/
	sprintf(c, "%d", *lineNumber);
	strcat(newMsg, c);
	strcat(newMsg, continuedMsg);
	strcat(newMsg, command);

	errorObj = errorMemoryAllocated(errorObj, flagFirstError);
	(*errorObj)->error = (char*)malloc(strlen(newMsg) * sizeof(char) + 1);
	strcpy((*errorObj)->error, newMsg);
}

void checkErrorsWithTwoOperhands(char *source, char *destiny, int *lineNumber, char *command, int *errorSizeCounter, Error **errorObj, int *flagFirstError)
{
	if (strcmp(command, "mov") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	if (strcmp(command, "add") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	if (strcmp(command, "add") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "lea") == '\0' && (strstr(source, "#") != '\0' || strstr(destiny, "#") != '\0' || (source[0] == 'r' && (source[1] >= '1' && source[1] <= '7'))))
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	if (strcmp(command, "not") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "clr") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "inc") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "dec") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "jmp") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "bne") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "red") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "prn") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "jsr") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "rts") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "stop") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}

}

void checkErrorsWithOneOperhand(char *destiny, int *lineNumber, char *command, int *errorSizeCounter, Error **errorObj, int *flagFirstError)
{
	if (strcmp(command, "mov") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "add") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "sub") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "lea") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	if (strcmp(command, "not") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "clr") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "inc") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "dec") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "jmp") == '\0' && (strstr(destiny, "#") != '\0' || (strstr(destiny, "[") != '\0')))
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "bne") == '\0' && (strstr(destiny, "#") != '\0' || (strstr(destiny, "[") != '\0')))
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "red") == '\0' && strstr(destiny, "#") != '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "jsr") == '\0' && (strstr(destiny, "#") != '\0' || (strstr(destiny, "[") != '\0')))
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "rts") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
	else if (strcmp(command, "stop") == '\0')
	{
		error(lineNumber, command, errorObj, flagFirstError);
		*errorSizeCounter += 1;
	}
}

Error** errorMemoryAllocated(Error** errorObj, int* flagFirstError)
{
	if (*errorObj == '\0')
	{
		*errorObj = (Error*)malloc(sizeof(Error));
		(*errorObj)->next = '\0';
	}
	if (*errorObj != '\0' && *flagFirstError != 1)
	{
		(*errorObj)->next = (Error*)malloc(sizeof(Error));
		*errorObj = (*errorObj)->next;
		(*errorObj)->next = '\0';
	}
	*flagFirstError = 0;
	return errorObj;
}
