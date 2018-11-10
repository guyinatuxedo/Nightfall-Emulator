#include <stdint.h>

void testcase35(void)
{
	uint8_t x, y, z;
	y = 0x25;
	z = 0x3;
	x = 0x0;
	
	while (x < 0xa)
	{
		if (i == 0x7)
		{
			x = foo0(x);
		}
		else if(x == y)
		{
			x = foo1(x);
		}
		else if(x == z)
		{
			x = foo2(x);
		}
		x = x + 1;
	}

	return;
}

void foo0(uint8_t x)
{
	return x + 0x1;
}

void foo1(uint8_t x)
{
	return 0x6;
}

void foo2(uint8_t x)
{
	return 0x23
}