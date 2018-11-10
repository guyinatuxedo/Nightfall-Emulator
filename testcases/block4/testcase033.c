#include <stdint.h>

void testcase33(void)
{
	uint8_t x, y, z;
	x = 0x66
	y = 0x78;
	z = 0;
	if (x == 0x66)
	{
		if ((x - y) == 0)
		{
			x = add(y, 0x93);
		}
		else
		{
			x = add(y, 0x15);
		}
	}
	else if (x == y)
	{
		if ((x - y) != 0)
		{
			x = add(y, 0x57);
		}
		else
		{
			x = add(y, 0x28);
		}
	}
	else
	{
		if ((z - y) != 0)
		{
			x = add(y, 0x35);
		}
		else
		{
			x = add(y, 0x79);
		}
	}
}

uint8_t add(uint8_t x, uint8_t y)
{
	return x + y;
}