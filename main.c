#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructures.h"

int checkForErrors(FILE *fpProg, Error **errorObj);
int firstTravel(FILE *fpProg, FILE *fpProgTemp, STObj **signTbl, DTObj **dataTbl, int *IC, int *DC, int sizeOfProgram);
void convert(Word *node, FILE *fpProg, FILE * fpw, FILE *fpwEntry, FILE *fpwExtern, int *IC, int *DC);
int checkIfEntryFile(char *fileName);
int checkIfExternFile(char *fileName);
void firstFunction(char* assemFileName);

int main(int argc, char* argv[])
{
	int i;
	for (i = 1; i<argc; i++)
		firstFunction(argv[i]);
return 0;
}

void firstFunction(char* assemFileName){
	FILE *fpProg, *fpProgTemp, *fpw, *fpwEntry, *fpwExtern;
	int *IC, *DC, errors = 0, flagForEntry, flagForExtern, sizeOfProgram = 0;
	char *programFileName, *objectsFileName, *externalsFileName, *entriesFileName;
	Word *node;
	STObj *signTbl = '\0';
	DTObj *dataTbl = '\0';
	Error *error = '\0';



	/*setting file names:*/
	programFileName = (char*)malloc(strlen(assemFileName)*sizeof(char) + 4);
	objectsFileName = (char*)malloc(strlen(assemFileName)*sizeof(char) + 4);
	externalsFileName = (char*)malloc(strlen(assemFileName)*sizeof(char) + 5);
	entriesFileName = (char*)malloc(strlen(assemFileName)*sizeof(char) + 5);
	
	strcpy(programFileName, assemFileName);
	strcpy(objectsFileName, assemFileName);
	strcpy(externalsFileName, assemFileName);
	strcpy(entriesFileName, assemFileName);

	strcat(programFileName, ".as");
	strcat(objectsFileName, ".ob");
	strcat(externalsFileName, ".ext");
	strcat(entriesFileName, ".ent");
	/*end setting file names*/

	IC = (int*)calloc(1, sizeof(int));
	DC = (int*)calloc(1, sizeof(int));
	*IC = 100;
	*DC = 0;
	error = (Error*)malloc(sizeof(Error));
	error->next = '\0';
	signTbl = (STObj*)malloc(sizeof(STObj));
	signTbl->next = '\0';
	dataTbl = (DTObj*)malloc(sizeof(DTObj));
	dataTbl->next = '\0';
	headSignTbl = signTbl;
	headDataTbl = dataTbl;
	headError = error;
	node = (Word*)malloc(sizeof(Word));
	node->next = '\0';


	

	/*check for errors*/
	fpProg = fopen(programFileName, "r");
	if (fpProg == '\0') printf("error opening file");
	errors = checkForErrors(fpProg, &error);
	fclose(fpProg);
	/*end errors check*/

	if (errors == 0)
	{
		/*start first travel*/
		fpProg = fopen(programFileName, "r");
		if (fpProg == '\0') printf("error opening file");
		fpProgTemp = fopen(programFileName, "r");
		if (fpProgTemp == '\0') printf("error opening file");

		sizeOfProgram = firstTravel(fpProg, fpProgTemp, &signTbl, &dataTbl, IC, DC, sizeOfProgram);
		fclose(fpProg);
		/*end first travel*/

		/*start second travel*/
		fpProg = fopen(programFileName, "r");
		if (fpProg == '\0') printf("error opening file");

		flagForEntry = checkIfEntryFile(programFileName);
		if (flagForEntry == 1)
		{
			fpwEntry = fopen(entriesFileName, "w+");
			if (fpwEntry == '\0') printf("error opening file");
		}
		else
			fpwEntry = '\0';

		flagForExtern = checkIfExternFile(programFileName);

		if (flagForExtern == 1)
		{
			fpwExtern = fopen(externalsFileName, "w+");
			if (fpwEntry == '\0') printf("error opening file");
		}
		else
			fpwExtern = '\0';
		fpw = fopen(objectsFileName, "w+");
		if (fpw == '\0') printf("error opening file");

		fprintf(fpw, "%d %d\n", *IC - 100, *DC);

		*IC = 100;
		convert(node, fpProg, fpw, fpwEntry, fpwExtern, IC, DC);
		fclose(fpProg);
		fclose(fpProgTemp);
		fclose(fpw);
		if (flagForEntry == 1)
			fclose(fpwEntry);
		if (flagForExtern == 1)
			fclose(fpwExtern);
		/*end second travel*/
	}


	free(IC);
	free(DC);
	free(error);
	free(signTbl);
	free(dataTbl);
	free(node);
	free(programFileName);
	free(objectsFileName);
	free(externalsFileName);
	free(entriesFileName);


}


	
