#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// This code contains functions for setting flags after operations

void auxiliaryFlag(cpu8080 *cpu, uint8_t x, uint8_t y, uint8_t z)
{
	if (z == 0)
	{
		if (((x & 0x8) == 0) && (y & 0x8))
		{
			cpu->aux = true;
		}
		else
		{
			cpu->aux = false;
		}
	}
	else if (z == 1)
	{
		if ((x & 0x8) && (y & 0x8))
		{
			cpu->aux = true;			
		}
		else
		{
			cpu->aux = false;
		}
	}
}

void zeroFlag(cpu8080 *cpu, uint16_t x)
{
	if ((x & 0xff) == 0)
	{
		cpu->zero = true;
	}
	else
	{
		cpu->zero = false;
	}
}

void signFlag(cpu8080 *cpu, uint16_t x)
{
	if ((x & 0x80) != 0)
	{
		cpu->sign = true;
	}
	else
	{
		cpu->sign = false;
	}
}

void parityFlag(cpu8080 *cpu, uint16_t x)
{
	int i, y, z;
	z = 0;
	for (i = 0; i < 8; i++)
	{
		y = x >> i;

		if (y & 0x1)
		{
			z += 1;
		}
	}	
	if ((z % 2) == 0)
	{
		cpu->parity = true;
	}
	else
	{
		cpu->parity = false;
	}
}

void carryFlag(cpu8080 *cpu, uint16_t x)
{
	if (x & 0x100)
	{
		cpu->carry = true;
	}
	else
	{
		cpu->carry = false;
	}
}

void carryFlagSub(cpu8080 *cpu, uint16_t x)
{
	if (x & 0x100)
	{
		cpu->carry = false;
	}
	else
	{
		cpu->carry = true;
	}
}

void carryFlag16(cpu8080 *cpu, uint32_t x)
{
	if ((x & 0x10000) != 0)
	{
		cpu->carry = true;
	}
	else
	{
		cpu->carry = false;
	}
}
