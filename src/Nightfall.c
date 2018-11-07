#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "Nightfall.h"

void menu(cpu8080 *cpu)
{
	uint16_t breakpoint;

	char *menuChoice;
	menuChoice = malloc(menuInputSize);
	while (1)
	{
		printf("ndb> ");
		fgets(menuChoice, menuInputSize - 1, stdin);
		switch (menuChoice[0])
		{
			case 'a':
				activateBreakpoint(cpu, menuChoice);
				break;
			case 'b':
				createBreakpoint(cpu, menuChoice);
				break;
			case 'c':
				emulate(cpu);
				break;
			case 'd':
				deactivateBreakpoint(cpu, menuChoice);
				break;
			case 'e':
				disassembleAll(cpu);
				break;
			case 'h':
				printHelp();
				break;
			case 'i':
				printRegisters(cpu);
				break;
			case 'j':
				jumpExec(cpu, menuChoice);
				break;
			case 'm':
				printMemoryRegions(cpu);
				break;
			case 'p':
				printRegister(cpu, menuChoice);
				break;
			case 'q':
				resetExecution(cpu);
				destroyCpu(cpu);
				return;
				break;
			case 'r':
				resetExecution(cpu);
				emulate(cpu);
				break;
			case 's':
				stepInstruction(cpu);
				emulate(cpu);
			case 't':
				toggleDisassExec(cpu);
				break;
			case 'u':
				dumpMemory(cpu);
				break;
			case 'x':
				examineMemory(cpu, menuChoice);
				break;
			case 'z':
				showBreakpoints(cpu->breakpoints, 0);
				break;
		}
	}
}

int main(int argc, char **argv)
{
	cpu8080 *cpu;
	char *binaryCode;

	if (argc != 2)
	{
		puts("gimme input file");
		exit(0);
	}

	cpu = createCpu();

	scanBinary(argv[1], cpu);

	puts("Nightfall 8080 Emualtor / Debugger / Disassembler");

	menu(cpu);

	return 0;
}