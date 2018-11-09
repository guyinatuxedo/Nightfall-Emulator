#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "Nightfall.h"

// This file holds functions which help with various parts of the code

uint8_t convertFlagsRegister(cpu8080 *cpu)
{
	uint8_t x = 0x2;
	x = x | ((uint8_t)(cpu->sign == true) << 7);
	x = x | ((uint8_t)(cpu->zero == true) << 6);
	x = x | ((uint8_t)(cpu->aux == true) << 4);
	x = x | ((uint8_t)(cpu->parity == true) << 2);
	x = x | ((uint8_t)(cpu->carry == true) << 1);
	return x;
}

int uintConvert(uint8_t x, uint8_t y)
{
	uint16_t z = 0;

	z = z | x;
	z = z << 8;
	z = z | y;

	return z;
}

int charToInt(char * inp)
{
	char *arg;
	int x;

	arg = removeBeginning(inp);
	if (strncmp(arg, "0x", 2) == 0)
	{
		x = strtoumax(arg, NULL, 16);

	}
	else
	{
		x = strtoumax(arg, NULL, 10);
	}	
	free(arg);
	return x;
}

char *removeBeginning(char *inp)
{
	int x, i;
	char *result;

	x = strlen(inp);
	result = malloc(x);
	for (i = 2; i < x; i++)
	{
		result[i - 2] = inp[i];
	}
	return result;
}

int hexDecimaltoInt(char *arg)
{
	int x;
	if (strncmp(arg, "0x", 2) == 0)
	{
		x = strtoumax(arg, NULL, 16);

	}
	else
	{
		x = strtoumax(arg, NULL, 10);
	}	
	return x;
}

void printTwoArgs(cpu8080 *cpu, int pc)
{
	uint8_t x = cpu->memory[pc + 1];
	uint8_t y = cpu->memory[pc + 2];	
	if ((x > 0xf) && (y > 0xf))
	{
		printf("0x%x%x\n", x, y);
		return;
	}
	else if ((x < 0xf) && (y < 0xf))
	{
		printf("0x0%x0%x\n", x, y);
		return;		
	}
	else if (x < 0xf)
	{
		printf("0x0%x%x\n", x, y);
		return;		
	}
	else
	{
		printf("0x%x0%x\n", x, y);
		return;		
	}
}

void printTwoArgsBack(cpu8080 *cpu, int pc)
{
	uint8_t x = cpu->memory[pc + 1];
	uint8_t y = cpu->memory[pc + 2];	
	if ((x > 0xf) && (y > 0xf))
	{
		printf("0x%x%x\n", y, x);
		return;
	}
	else if ((x < 0xf) && (y < 0xf))
	{
		printf("0x0%x0%x\n", y, x);
		return;		
	}
	else if (y < 0xf)
	{
		printf("0x0%x%x\n", y, x);
		return;		
	}
	else
	{
		printf("0x%x0%x\n", y, x);
		return;		
	}
}

void setSpecificFlagPsw(cpu8080 *cpu, int x, int cmp, bool *flag)
{
	if (x & cmp)
	{
		*flag = true;
	}
	else
	{
		*flag = false;
	}
}

void setCode(cpu8080 *cpu, char *code, int size)
{
	memcpy(cpu->memory, code, size);
	cpu->codeEnd = (size - 1);
	cpu->codeBeg = 0x0;
	return;
}

void scanBinary(char *fileName, cpu8080 *cpu)
{
	FILE *fp;
	char *code;
	int size;
	int bytesRead;

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		puts("File not found");
		exit(0);
	}

	size = getFileSize(fp);
	code = calloc(1, (size + 1));

	fseek(fp, 0, SEEK_SET);	

	bytesRead = fread(code, sizeof(char), size, fp);

	fclose(fp);

	memcpy(cpu->memory, code, size);
	cpu->codeBeg = 0x0;
	cpu->codeEnd = (uint16_t)(size - 0x1);
	free(code);
	return;
}

long int getFileSize(FILE *fp)
{
	long int size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	return size;
}