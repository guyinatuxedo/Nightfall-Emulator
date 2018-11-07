#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "Nightfall.h"

void mvi(cpu8080 *cpu, uint8_t *reg, uint8_t arg)
{
	*(reg) = arg;
	cpu->pc += 1;
}

void ldax(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	printf("loading 0x%x from 0x%x\n", cpu->memory[uintConvert(x, y)], uintConvert(x, y));
	cpu->a = (uint8_t)cpu->memory[uintConvert(x, y)];
}

void stax(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	cpu->memory[uintConvert(x, y)] = cpu->a;	
}