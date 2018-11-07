#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

int isBreakPoint(cpu8080 *cpu)
{
	if (cpu->breakpoints == NULL)
	{
		return 1;
	}
	else
	{
		return checkBreakPoint(cpu->breakpoints, cpu->pc);
	}
}

breakpoints *findLastBreakpoint(breakpoints *breakpoint)
{
	if (breakpoint == NULL)
	{
		return NULL;
	}
	else
	{
		if (breakpoint->next == NULL)
		{
			return breakpoint;
		}
		else
		{
			return findLastBreakpoint(breakpoint->next);
		}
	}
}

breakpoints *allocateBreakpoint(uint16_t x)
{
	breakpoints *b;
	b = malloc(sizeof(breakpoints));
	b->breakp = x;
	b->next = NULL;
	return b;
}

int checkBreakPoint(breakpoints * breakpt, uint16_t x)
{
	if (breakpt->breakp == x && breakpt->active == true)
	{
		return 0;
		puts("sam");
	}
	else if (breakpt->next != NULL)
	{
		return checkBreakPoint(breakpt->next, x);
	}
	else
	{
		return 1;
	}
}