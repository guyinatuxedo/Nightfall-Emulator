#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// This file holds code for push / pop functions

void pop(cpu8080 *cpu, uint8_t *x, uint8_t *y)
{
	*y = cpu->memory[cpu->sp];
	cpu->sp += 1;
	*x = cpu->memory[cpu->sp];
	cpu->sp += 1;
}

void push(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	cpu->sp -= 1;
	cpu->memory[cpu->sp] = x;
	cpu->sp -= 1;
	cpu->memory[cpu->sp] = y;
}

void push16(cpu8080 *cpu, uint16_t x)
{
	uint8_t y;
	y = (uint8_t)(x >> 8);
	cpu->sp -= 1;
	cpu->memory[cpu->sp] = y;
	cpu->sp -= 1;
	y = (uint8_t)(x);
	cpu->memory[cpu->sp] = y;
}

void popPsw(cpu8080 *cpu)
{
	uint8_t x;
	x = cpu->memory[cpu->sp];
	setSpecificFlagPsw(cpu, x, 0x1, &(cpu->carry));
	setSpecificFlagPsw(cpu, x, 0x4, &(cpu->parity));
	setSpecificFlagPsw(cpu, x, 0x10, &(cpu->aux));
	setSpecificFlagPsw(cpu, x, 0x40, &(cpu->zero));
	setSpecificFlagPsw(cpu, x, 0x80, &(cpu->sign));
	cpu->sp += 1;
	cpu->a = cpu->memory[cpu->sp];
	cpu->sp += 1;	
}