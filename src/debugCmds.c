#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "Nightfall.h"

// This source file holds functions which deal with various debugger commands

void activateBreakpoint(cpu8080 *cpu, char *menuInput)
{
	int i, n;
	breakpoints *b;
	b = cpu->breakpoints;

	if (b == NULL)
	{
		puts("There are no breakpoints.");
		return;
	}

	n = charToInt(menuInput);
	for (i = 0; i < n; i++)
	{
		b = b->next;
		if (b == NULL)
		{
			puts("That breakpoint doesn't exist");
			return;
		}
	}

	b->active = true;
}

void createBreakpoint(cpu8080 *cpu, char *menuInput)
{
	char *arg;
	int x;
	breakpoints *last, *next;
	last = findLastBreakpoint(cpu->breakpoints);

	x = charToInt(menuInput);

	next = allocateBreakpoint((uint16_t) x);
	next->active = true;
	if (last == NULL)
	{
		cpu->breakpoints = next;
	}
	else
	{
		last->next = next;
	}
}

void deactivateBreakpoint(cpu8080 *cpu, char *menuInput)
{
	int i, n;
	breakpoints *b;
	b = cpu->breakpoints;

	if (b == NULL)
	{
		puts("There are no breakpoints.");
		return;
	}

	n = charToInt(menuInput);
	for (i = 0; i < n; i++)
	{
		b = b->next;
		if (b == NULL)
		{
			puts("That breakpoint doesn't exist");
			return;
		}
	}

	b->active = false;
}

void disassembleAll(cpu8080 *cpu)
{
	int i;
	i = 0;
	while (i < (cpu->codeEnd + 1))
	{
		i = disass(cpu, i);
	}
}

void printHelp(void)
{
	puts("Help Menu");
	puts("a <breakpoint id>	-	Activate a previously deacticated breakpoint");
	puts("b <code address>	-	Set a breakpoint at the address");
	puts("c			-	Continue execution from a breakpoint");
	puts("d <breakpoint id>	-	Deactivate a breakpoint");
	puts("e			-	Disassemble all code in binary");
	puts("h			-	Print help menu");
	puts("i			-	Display values of all registers and flags");
	puts("j <next adr>		-	Set the next address to be executed (essentially a jump, but need to continue)");
	puts("m			-	Print the addresses for memory regions");
	puts("p <register>		-	Prints contents of register");
	puts("q			-	Quit the emulator/debugger");
	puts("r			-	Run the emulator from the start");
	puts("s			-	Step to the next instruction");
	puts("t			-	Toggle running disassembly, which disassembles instructions as they are ran");
	puts("u			-	Dump the contents of memory to the file memory.dat");
	puts("x <amnt> <adr>		-	Print amnt 8 bit segments starting at adr (if single argument given, amnt is 1)");
	puts("y <target <value>	-	Set target register or address to value");
	puts("z 			-	Show all breakpoints, and if they are activated");
}

void printRegisters(cpu8080 *cpu)
{
	puts("8 bit registers");
	printf("register a: 0x%x\n", (int)cpu->a);
	printf("register b: 0x%x\n", (int)cpu->b);
	printf("register c: 0x%x\n", (int)cpu->c);
	printf("register d: 0x%x\n", (int)cpu->d);
	printf("register e: 0x%x\n", (int)cpu->e);
	printf("register h: 0x%x\n", (int)cpu->h);
	printf("register l: 0x%x\n", (int)cpu->l);

	puts("\n16 bit registers");
	printf("register bc: 0x%x\n", (int)uintConvert(cpu->b, cpu->c));
	printf("register de: 0x%x\n", (int)uintConvert(cpu->d, cpu->e));
	printf("register hl: 0x%x\n", (int)uintConvert(cpu->h, cpu->l));
	printf("register af: 0x%x\n", (int)uintConvert(cpu->a, convertFlagsRegister(cpu)));

	printFlags(cpu);

	puts("\nStack registers (16 bit)");
	printf("register pc: 0x%x\n", (int)cpu->pc);
	printf("register sp: 0x%x\n", (int)cpu->sp);

	printf("\nInterrupt bit: 0x%x\n", cpu->allowInterrupt);
	return;
}

void printFlags(cpu8080 *cpu)
{
	puts("\nFlags");
	printf("7: Sign flag: %d\n", (int)cpu->sign);
	printf("6: Zero flag: %d\n", (int)cpu->zero);
	printf("5: zero1 flag (value shouldn't change): %d\n", cpu->zero1);
	printf("4: Auxillary flag: %d\n", (int)cpu->aux);
	printf("3: zero2 flag (value shouldn't change): %d\n", cpu->zero2);
	printf("2: Parity flag: %d\n", (int)cpu->parity);
	printf("1: one flag (value shouldn't change): %d\n", cpu->one);
	printf("0: Carry flag: %d\n", (int)cpu->carry);
}

void jumpExec(cpu8080 *cpu, char *input)
{
	int instr;
	instr = charToInt(input);
	cpu->pc = instr - 1;
}

void printMemoryRegions(cpu8080 *cpu)
{
	printf("Total Memory Region:\t 0x%x - 0x%x\n", (int)cpu->memBeg, (int)cpu->memEnd);
	printf("Code Memory Region:\t 0x%x - 0x%x\n", (int)cpu->codeBeg, (int)cpu->codeEnd);
	printf("Stack Memory Region:\t 0x%x - 0x%x\n", (int)cpu->stackBeg, (int)cpu->sp);
}

void printRegister(cpu8080 *cpu, char *menuInput)
{
	char *arg;

	arg = removeBeginning(menuInput);	

	if (strncmp(arg, "bc", 2) == 0)
	{
		printf("register bc: 0x%x\n", (int)uintConvert(cpu->b, cpu->c));
	}

	else if (strncmp(arg, "de", 2) == 0)
	{
		printf("register de: 0x%x\n", (int)uintConvert(cpu->d, cpu->e));
	}	

	else if (strncmp(arg, "hl", 2) == 0)
	{
		printf("register hl: 0x%x\n", (int)uintConvert(cpu->h, cpu->l));
	}

	else if (strncmp(arg, "af", 2) == 0)
	{
		printf("register af: 0x%x\n", (int)uintConvert(cpu->h, convertFlagsRegister(cpu)));
	}

	else if (strncmp(arg, "pc", 2) == 0)
	{
		printf("register pc: 0x%x\n", (int)cpu->pc);
	}

	else if (strncmp(arg, "sp", 2) == 0)
	{
		printf("register sp: 0x%x\n", (int)cpu->sp);
	}

	else
	{
		switch(arg[0])
		{
			case 'a':
				printf("register a: 0x%x\n", (uint16_t)cpu->a);
				break;
			case 'b':
				printf("register b: 0x%x\n", (uint16_t)cpu->b);
				break;
			case 'c':
				printf("register c: 0x%x\n", (uint16_t)cpu->c);
				break;
			case 'd':
				printf("register d: 0x%x\n", (uint16_t)cpu->d);
				break;
			case 'e':
				printf("register e: 0x%x\n", (uint16_t)cpu->e);
				break;
			case 'h':
				printf("register h: 0x%x\n", (uint16_t)cpu->h);
				break;
			case 'l':
				printf("register l: 0x%x\n", (uint16_t)cpu->l);
				break;
			case 'f':
				printFlags(cpu);
				break;
		}
	}

	free(arg);
	return;
}

void resetExecution(cpu8080 *cpu)
{
	// Set the flags to their default values
	cpu->carry = 0;
	cpu->one = 1;
	cpu->parity = 0;
	cpu->zero = 0;
	cpu->aux = 0;
	cpu->zero1 = 0;
	cpu->sign = 0;

	// Set the registers to their default values
	cpu->a = 0x0;
	cpu->b = 0x0;
	cpu->c = 0x0;
	cpu->d = 0x0;
	cpu->e = 0x0;
	cpu->h = 0x0;
	cpu->l = 0x0;

	// Initialize the stack pointer, and the program counter
	cpu->sp = startStack;
	cpu->pc = 0x0;

	cpu->running = false;
	cpu->breaked = false;

	cpu->step = 0;
}

cpu8080 *createCpu(void)
{
	cpu8080 *cpu;

	// Allocate memory for the cpu
	cpu = calloc(1, sizeof(cpu8080));

	// Allocate the memory for the cpu
	cpu->memory = calloc(1, memorySize);

	// Set the flags to their default values
	cpu->carry = 0;
	cpu->one = 1;
	cpu->parity = 0;
	cpu->zero = 0;
	cpu->aux = 0;
	cpu->zero1 = 0;
	cpu->sign = 0;

	// Set the registers to their default values
	cpu->a = 0x0;
	cpu->b = 0x0;
	cpu->c = 0x0;
	cpu->d = 0x0;
	cpu->e = 0x0;
	cpu->h = 0x0;
	cpu->l = 0x0;

	// Initialize the stack pointer, and the program counter
	cpu->sp = (uint16_t)startStack;
	cpu->pc = 0x0;

	cpu->allowInterrupt = 1;

	cpu->breakpoints = NULL;

	cpu->running = false;

	cpu->breaked = false;

	// Set the values for the memory locations
	cpu->memBeg = (uint16_t)0;
	cpu->memEnd = (uint16_t)memorySize;
	cpu->stackBeg = (uint16_t)startStack;

	cpu->disasAsExec = false;

	return cpu;
}

void freeBreakpoints(breakpoints *breakpoint)
{
	breakpoints *next;
	if (breakpoint == NULL)
	{
		return;
	}
	else
	{
		next = breakpoint->next;
		free(breakpoint);
		freeBreakpoints(next);
	}
}

void destroyCpu(cpu8080 *cpu)
{
	free(cpu->memory);
	cpu->memory = 0;
	freeBreakpoints(cpu->breakpoints);
	free(cpu);
	cpu = 0;
	return;
}

void stepInstruction(cpu8080 *cpu)
{
	cpu->step = 1;
}

void toggleDisassExec(cpu8080 *cpu)
{
	if (cpu->disasAsExec == false)
	{
		cpu->disasAsExec = true;
	}	
	else
	{
		cpu->disasAsExec = false;
	}
}

void dumpMemory(cpu8080 *cpu)
{
	FILE *fp = fopen(memoryOutputFile, "w");
	if (fp == NULL)
	{
		puts("Could not open output file");
	}

	fwrite(cpu->memory, sizeof(uint8_t), memorySize, fp);
	fclose(fp);
	return;
}

void examineMemory(cpu8080 *cpu, char *inp)
{
	char *arg0, *arg1;
	int x, y, i;

	arg0 = strtok(inp, " ");
	if (arg0 == NULL)
	{
		puts("No args");
		return;
	}

	arg0 = strtok(NULL, " ");
	if (arg0 == NULL)
	{
		puts("one args");
		return;
	}

	arg1 = strtok(NULL, " ");
	if (arg1 == NULL)
	{
		x = hexDecimaltoInt(arg0);
		printf("0x%x: 0x%x\n", x, cpu->memory[x]);
		return;
	}
	else
	{
		x = hexDecimaltoInt(arg0);
		y = hexDecimaltoInt(arg1);
		for (i = 0; i < x; i++)
		{
			printf("0x%x: 0x%x\n", y + i, cpu->memory[y + i]);
		}
		return;		
	}
}

void showBreakpoints(breakpoints *breakpoint, int n)
{
	if (breakpoint == NULL)
	{
		return;
	}
	else
	{
		printf("Breakpoint 0x%x at %x\t\t(active: %d)\n", n, breakpoint->breakp, (int)breakpoint->active);
		return showBreakpoints(breakpoint->next, n + 1);
	}
}

void set(cpu8080 *cpu, char *inp)
{
	char *arg0, *arg1;
	int x, y, i;

	arg0 = strtok(inp, " ");
	if (arg0 == NULL)
	{
		puts("This command requires two arguments.");
		return;
	}

	arg0 = strtok(NULL, " ");
	if (arg0 == NULL)
	{
		puts("This command requires two arguments.");
		return;
	}

	arg1 = strtok(NULL, " ");
	if (arg1 == NULL)
	{
		puts("This command requires two arguments.");
		return;
	}
	else
	{
		y = hexDecimaltoInt(arg1);
		switch (*arg0)
		{
			case 0x61:
				cpu->a = y;
				break;

			case 0x62:
				cpu->b = y;
				break;

			case 0x63:
				cpu->c = y;
				break;

			case 0x64:
				cpu->d = y;
				break;

			case 0x65:
				cpu->e = y;
				break;

			case 0x68:
				cpu->h = y;
				break;

			case 0x6c:
				cpu->l = y;
				break;

			default:
				if (strncmp(arg0, "sp", 2) == 0)
				{
					cpu->sp = y;
					return;
				}
				if (strncmp(arg0, "pc", 2) == 0)
				{
					cpu->pc = y;
					return;
				}
				x = hexDecimaltoInt(arg0);
				cpu->memory[x] = y;
				break;
		}
		return;		
	}
}