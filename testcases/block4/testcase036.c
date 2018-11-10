#include <stdint.h>

void testcase36(void)
{
	uint8_t x, y, z;
	y = 0x25;
	z = 0x3;
	x = 0x0;
	
	while (1)
	{
		switch (x)
		{
			case 0x2:
				x = dawn(x);
				break;
			case 0x17:
				x = 0x3;
				break;
			case 0x4:
				return;
				break;
			default:
				x = x + 1;

		}
	}
}

void dawn(uint8_t x)
{
	return x * 3;
}