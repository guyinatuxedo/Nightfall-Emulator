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

void inx8(uint8_t *x, uint8_t *y)
{
	if ((*y) < 0xff)
	{
		(*y) += 1;
	}

	else if ((*x) != 0xff)
	{
		(*y) = 0;
		(*x) += 1;
	}

	else
	{
		(*y) = 0;
		(*x) = 0;
	}
}

void inx16(uint16_t *x)
{
	if ((*x) != 0xffff)
	{
		(*x) += 1;
	}

	else
	{
		(*x) = 0;
	}
}

void dcx8(uint8_t *x, uint8_t *y)
{
	if ((*y) > 0x00)
	{
		(*y) -= 1;
	}
	else if ((*x) != 0x00)
	{
		(*y) = 0xff;
		(*x) -= 1;
	}
	else
	{
		(*y) = 0xff;
		(*x) = 0xff;
	}
}

void dcx16(uint16_t *x)
{
	if ((*x) != 0xffff)
	{
		(*x) = (*x) - 1;
	}
	else
	{
		(*x) = 0x0;
	}
}

void dad(cpu8080 *cpu, uint8_t *x, uint8_t *y)
{
	uint16_t x1, y1;
	uint32_t z;

	x1 = (uint16_t)(*x);
	x1 = x1 << 8;
	x1 = x1 | (uint16_t)(*y);

	y1 = (uint16_t)cpu->h;
	y1 = y1 << 8;
	y1 = y1 | (uint16_t)cpu->l;

	z = x1 + y1;

	carryFlag16(cpu, z);
	cpu->l = (uint8_t)(z);
	cpu->h = (uint8_t)(z >> 8);
}

void dad16(cpu8080 *cpu, uint16_t *z)
{
	uint16_t x, y;
	uint32_t z1;

	x = (uint16_t)(*z);
	y = (uint16_t)cpu->h;
	y = y << 8;
	y = y | (uint16_t)cpu->l;
	
	z1 = x + y;
	
	carryFlag16(cpu, z1);
	cpu->l = (uint8_t)(z1);
	cpu->h = (uint8_t)(z1 >> 8);
}