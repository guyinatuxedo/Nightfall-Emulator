void testcase32(void)
{
	int x, y, z;
	x = 0x66
	y = 0x78;
	z = 0;
	if (x == 0x66)
	{
		if ((x - y) == 0)
		{
			x = 0x15;
		}
		else
		{
			x = 0x93;
		}
	}
	else if (x == y)
	{
		if ((x - y) != 0)
		{
			x = 0x57;
		}
		else
		{
			x = 0x28;
		}
	}
	else
	{
		if ((z - y) != 0)
		{
			x = 0x35;
		}
		else
		{
			x = 0x79;
		}
	}
}
