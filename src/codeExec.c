#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// Here is the code for operations dealing with comparisons, and redirecting code execution

void cmp(cpu8080 *cpu, uint8_t x)
{
	uint8_t y = ((~x) + 1);

	uint16_t z = (cpu->a + y);

	carryFlagSub(cpu, z);

	signFlag(cpu, z);

	zeroFlag(cpu, z);

	parityFlag(cpu, z);

	auxiliaryFlag(cpu, cpu->a, x, 0);
} 

void jmp(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	uint16_t z;
	z = (uint16_t)y;
	z = z << 8;
	z = z | x;
	cpu->pc = (z - 1);
}

void call(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	uint16_t z;
	z = (y << 8);
	z = z | x;
	z = z - 1;
	push16(cpu, (cpu->pc + 3));
	cpu->pc = z;
}

void ret(cpu8080 *cpu)
{
	uint8_t x;
	uint16_t y;
	x = (uint16_t)cpu->memory[cpu->sp];
	cpu->sp += 1;
	y = (uint16_t)cpu->memory[cpu->sp];
	cpu->sp += 1;
	y = (y << 8);
	y = y | x;
	cpu->pc = y - 1;
}

void rst(cpu8080 *cpu, uint16_t x)
{
	push16(cpu, (cpu->pc+1));
	cpu->pc = x - 1;
}