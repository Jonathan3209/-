#include<stdio.h>
#include <stdlib.h>
#include <string.h>


char* removeSpaces(char *line)
{
	int i = 0, j = 0, len = strlen(line);

	for (i = 0, j = 0; i < len; i++)
	{
		if (line[i] == ' ' && line[i] != '\0')
		{
			for (j = i; j < len; j++)
			{
				line[j] = line[j + 1];
			}
			len--;
		}
	}
	return line;
}


char* removeSpacesInSource(char *line)
{
	int i = 5, j = 5, len = strlen(line);

	for (i = 5, j = 5; i < len; i++)
	{
		if (line[i] == ' ' && line[i] != NULL)
		{
			for (j = i; j < len; j++)
			{
				line[j] = line[j + 1];
			}
			len--;
		}
	}
	return line;
}

char* removeComment(char *line)
{
	char *token;
	token = strtok(line, ";");
	return token;
}

void handleTabs(char *line)
{
	char *token, fHalf[25], sHalf[25];
	token = strtok(line, "\t");
	strcpy(fHalf, token);
	if (strstr(token, ":") != '\0')
	{
		token = strtok('\0', "\t");
		strcpy(sHalf, token);
		strcat(fHalf, " ");
		strcat(fHalf, sHalf);
	}
	strcpy(line, fHalf);
}

char* decToBinary(int num, int size, char *finalString) /*decimal to binary with leading zeros*/
{
	int index, i, *bin;
	char ch[2];

	bin = (int*)calloc(size, sizeof(int));

	index = size - 1;
	while (index >= 0)
	{
		/* Store LSB of num to bin */
		bin[index] = num & 1;

		/* Decrement index */
		index--;

		/* Right Shift num by 1 */
		num >>= 1;
	}
	if (bin[0] == 1)
		strcpy(finalString, "1");
	else
		strcpy(finalString, "0");
	for (i = 0; i < size; i++)
	{
		/*itoa(bin[i], ch, 10);*/
		sprintf(ch, "%d", bin[i]);
		if (i != 0) /*beacause finalString has '0' in it (skip the first '0')*/
			strcat(finalString, ch);
	}
	free(bin);
	return finalString;
}

int checkSizeOfBinaryNumber(int num)
{
	int counter = 0;
	while (num != 0)
	{
		num /= 2;
		counter++;
	}
	return counter;
}

char* decToTwosComplement(char *binary, int size)
{
	char *onesComp, *twosComp;
	int i, carry = 1;
	onesComp = (char*)malloc(size * sizeof(char) + 1);
	twosComp = (char*)malloc(size * sizeof(char) + 1);
	/* Find ones complement of the binary number */
	for (i = 0; i < size; i++)
	{
		if (binary[i] == '1')
		{
			onesComp[i] = '0';
		}
		else if (binary[i] == '0')
		{
			onesComp[i] = '1';
		}
	}
	onesComp[size] = '\0';

	/*
	 * Add 1 to the ones complement
	 */
	for (i = size - 1; i >= 0; i--)
	{
		if (onesComp[i] == '1' && carry == 1)
		{
			twosComp[i] = '0';
		}
		else if (onesComp[i] == '0' && carry == 1)
		{
			twosComp[i] = '1';
			carry = 0;
		}
		else
		{
			twosComp[i] = onesComp[i];
		}
	}
	twosComp[size] = '\0';
	return twosComp;
}
