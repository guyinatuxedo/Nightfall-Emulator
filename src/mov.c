#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// Here is the code for mov (and mov - like) operationa

void mvi(cpu8080 *cpu, uint8_t *reg, uint8_t arg)
{
	*(reg) = arg;
	cpu->pc += 1;
}

void ldax(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	cpu->a = (uint8_t)cpu->memory[uintConvert(x, y)];
}

void stax(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	cpu->memory[uintConvert(x, y)] = cpu->a;	
}

void lxi(cpu8080 *cpu, uint8_t *x, uint8_t *y)
{
	(*x) = cpu->memory[cpu->pc + 1];
	(*y) = cpu->memory[cpu->pc + 2];
	cpu->pc += 2;
}

void lxi16(cpu8080 *cpu, uint16_t *x)
{
	uint16_t y;
	y = cpu->memory[cpu->pc + 1];
	y = y << 8;
	y = y | cpu->memory[cpu->pc + 2]; 
	(*x) = y;
	cpu->pc += 2;
}