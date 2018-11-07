#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// This file has functions which handle various math operations

void add(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)x + (cpu->a));

	carryFlag(cpu, z);

	signFlag(cpu, z);

	zeroFlag(cpu, z);

	parityFlag(cpu, z);

	auxiliaryFlag(cpu, cpu->a, x, 1);

	cpu->a = (uint8_t)z;
}

void adc(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)x + (cpu->a) + ((uint16_t)cpu->carry));

	carryFlag(cpu, z);

	signFlag(cpu, z);

	zeroFlag(cpu, z);

	parityFlag(cpu, z);

	auxiliaryFlag(cpu, cpu->a, x, 1);

	cpu->a = (uint8_t)z;
}

void sub(cpu8080 *cpu, uint8_t x)
{
	uint8_t y = ((~x) + 1);

	uint16_t z = (cpu->a + y);

	carryFlagSub(cpu, z);

	signFlag(cpu, z);

	zeroFlag(cpu, z);

	parityFlag(cpu, z);

	auxiliaryFlag(cpu, cpu->a, x, 0);

	cpu->a = (uint8_t)z;
}

void sbb(cpu8080 *cpu, uint8_t x)
{
	uint8_t y = ((~(x + (int)cpu->carry)) + 1);

	uint16_t z = (cpu->a + y);

	carryFlagSub(cpu, z);

	signFlag(cpu, z);

	zeroFlag(cpu, z);

	parityFlag(cpu, z);

	auxiliaryFlag(cpu, cpu->a, x, 0);

	cpu->a = (uint8_t)z;
}

void and(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a & x;

	cpu->carry = false;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void xor(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a ^ x;

	cpu->carry = false;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->aux = false;

	cpu->a = (uint8_t)z;
}

void or(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a | x;

	cpu->carry = false;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void inr(cpu8080 *cpu, uint8_t *reg)
{
	(*reg) += 1;
	zeroFlag(cpu, *reg);
	signFlag(cpu, *reg);
	parityFlag(cpu, *reg);
	auxiliaryFlag(cpu, *reg, 1, 1);
	carryFlag(cpu, *reg);
}

void dcr(cpu8080 *cpu, uint8_t *reg)
{
	(*reg) -= 1;
	zeroFlag(cpu, *reg);
	signFlag(cpu, *reg);
	parityFlag(cpu, *reg);
	auxiliaryFlag(cpu, *reg, 1, 0);
	carryFlag(cpu, *reg);
}