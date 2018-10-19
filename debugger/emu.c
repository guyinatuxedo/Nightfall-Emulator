#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#define menuInputSize 20
#define memorySize 0xffff
#define startStack 0x4000

typedef struct breakpoints {
	uint16_t breakp;
	bool active;
	struct breakpoints *next;
} breakpoints;

typedef struct cpu8080 {
	// Arithmetic register
	uint8_t	a;

	// BC register
	uint8_t	b;
	uint8_t	c;

	// DE register
	uint8_t	d;
	uint8_t	e;

	// HL register
	uint8_t	h;
	uint8_t l;

	// Flags
	bool carry;
	bool one;
	bool parity;
	bool zero1;
	bool aux;
	bool zero2;
	bool zero;
	bool sign;

	// stack (sp) and program counter (pc) registers
	uint16_t sp;
	uint16_t pc;

	// a ptr to the memory region
	uint8_t *memory;

	// a ptr to the breakpoints
	breakpoints *breakpoints;

	// a bool to indicate if it is running
	bool running;

	bool breaked;

	// Here are some values, which will keep track of where the various regions of memeory are 
	uint16_t memBeg;	// Beginning of entire memory region
	uint16_t memEnd;	// End of entrie memory region
	uint16_t codeBeg;	// Beginning of where code is loaded
	uint16_t codeEnd;
	uint16_t stackBeg;	// Beginning of the stack
	// the end of the code is kept by codeEnd, and the end of the stack is kep by sp

} cpu8080;

cpu8080 *createCpu(void)
{
	cpu8080 *cpu;

	// Allocate memory for the cpu
	cpu = calloc(1, sizeof(cpu8080));

	// Allocate the memory for the cpu
	cpu->memory = calloc(1, memorySize);

	// Set the flags to their default values
	cpu->carry = 0;
	cpu->one = 1;
	cpu->parity = 0;
	cpu->zero = 0;
	cpu->aux = 0;
	cpu->zero1 = 0;
	cpu->sign = 0;

	// Set the registers to their default values
	cpu->a = 0x0;
	cpu->b = 0x0;
	cpu->c = 0x0;
	cpu->d = 0x0;
	cpu->e = 0x0;
	cpu->h = 0x0;
	cpu->l = 0x0;

	// Initialize the stack pointer, and the program counter
	cpu->sp = (uint16_t)startStack;
	cpu->pc = 0x0;

	cpu->breakpoints = NULL;

	cpu->running = false;

	cpu->breaked = false;

	// Set the values for the memory locations
	cpu->memBeg = (uint16_t)0;
	cpu->memEnd = (uint16_t)memorySize;
	cpu->stackBeg = (uint16_t)startStack;

	return cpu;
}

void setCode(cpu8080 *cpu, char *code, int size)
{
	memcpy(cpu->memory, code, size);
	cpu->codeEnd = (size - 1);
	cpu->codeBeg = 0x0;
	return;
}

long int getFileSize(FILE *fp)
{
	long int size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	return size;
}

void scanBinary(char *fileName, cpu8080 *cpu)
{
	FILE *fp;
	char *code;
	int size;
	int bytesRead;

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		puts("File not found");
		exit(0);
	}

	size = getFileSize(fp);
	code = calloc(1, (size + 1));

	fseek(fp, 0, SEEK_SET);	

	bytesRead = fread(code, sizeof(char), size, fp);

	fclose(fp);

	memcpy(cpu->memory, code, size);
	cpu->codeBeg = 0x0;
	cpu->codeEnd = (uint16_t)(size - 0x1);
	return;
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

void zeroFlag(cpu8080 *cpu, uint16_t x)
{
	if (x == 0)
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

void carryFlag(cpu8080 *cpu, uint16_t x)
{
	if ((x & 0x100) == 0)
	{
		cpu->carry = true;
	}
	else
	{
		cpu->carry = false;
	}
}

void carryFlag16(cpu8080 *cpu, uint16_t x)
{
	if ((x & 0x10000) == 0)
	{
		cpu->carry = true;
	}
	else
	{
		cpu->carry = false;
	}
}

void cmp(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	uint16_t z = (uint16_t)x - y;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

} 

void or(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a | x;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void and(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a & x;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void xor(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = (uint16_t)cpu->a ^ x;

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void add(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)cpu->a + (cpu->b));

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void adc(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)cpu->a + (cpu->b + ((int)cpu->carry)));

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void sub(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)cpu->a - (cpu->b));

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void sbb(cpu8080 *cpu, uint8_t x)
{
	uint16_t z = ((uint16_t)cpu->a - (cpu->b + ((int)cpu->carry)));

	zeroFlag(cpu, z);

	signFlag(cpu, z);

	carryFlag(cpu, z);

	parityFlag(cpu, z);

	cpu->a = (uint8_t)z;
}

void push(cpu8080 *cpu, uint16_t x)
{
	uint8_t y;
	y = (uint8_t)(x >> 8);
	cpu->sp -= 1;
	cpu->memory[cpu->sp] = y;
	cpu->sp -= 1;
	y = (uint8_t)(x);
	cpu->memory[cpu->sp] = y;
}

void pop(cpu8080 *cpu, uint8_t *x, uint8_t *y)
{
	*y = cpu->memory[cpu->sp];
	cpu->sp += 1;
	*x = cpu->memory[cpu->sp];
	cpu->sp += 1;
}

void ret(cpu8080 *cpu)
{
	uint8_t x;
	uint16_t y;
	x = (uint16_t)cpu->memory[cpu->sp];
	cpu->sp += 1;
	y = (uint16_t)cpu->memory[cpu->sp];
	cpu->sp += 1;
	y = (y << 8);
	y = y | x;
	cpu->pc = y;
}

void jmp(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	uint16_t z;
	z = (uint16_t)x;
	z = z << 8;
	z = z | y;
	cpu->pc = z;
}

void call(cpu8080 *cpu, uint8_t x, uint8_t y)
{
	uint16_t z;
	z = (uint16_t)x;
	z = (x << 8);
	z = z | y;
	push(cpu, (cpu->pc + 3));
	cpu->pc = z;
}

void rst(cpu8080 *cpu, uint16_t x)
{
	push(cpu, (cpu->pc+1));
	cpu->pc = x;
}

int uintConvert(uint8_t x, uint8_t y)
{
	uint16_t z = 0;

	z = z | x;
	z = z << 8;
	z = z | y;

	return z;
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

void resetExecution(cpu8080 *cpu)
{
	// Set the flags to their default values
	cpu->carry = 0;
	cpu->one = 1;
	cpu->parity = 0;
	cpu->zero = 0;
	cpu->aux = 0;
	cpu->zero1 = 0;
	cpu->sign = 0;

	// Set the registers to their default values
	cpu->a = 0x0;
	cpu->b = 0x0;
	cpu->c = 0x0;
	cpu->d = 0x0;
	cpu->e = 0x0;
	cpu->h = 0x0;
	cpu->l = 0x0;

	// Initialize the stack pointer, and the program counter
	cpu->sp = startStack;
	cpu->pc = 0x0;

	cpu->running = false;
	cpu->breaked = false;
}

void emulate(cpu8080 *cpu)
{
	int running;
	uint8_t op;

	uint8_t x0;
	uint16_t x;
	uint16_t y;
	uint32_t z;

	running = 0;
	for (cpu->pc = cpu->pc; cpu->pc < 0x1000 && running == 0; cpu->pc++)
	{
		op = (uint8_t)cpu->memory[cpu->pc];
		printf("%x\n", op);
		if (isBreakPoint(cpu) == 0 && cpu->breaked == false)
		{
			printf("Breakpoint hit at 0x%x\n", (int)cpu->pc);
			cpu->breaked = true;
			return;
		}
		else
		{
			cpu->breaked = false;
		}

		switch (op)
		{
			// nop
			case 0x00:
				break;

			// lxi b, x
			case 0x01:
				cpu->b = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// stax b
			case 0x02:
				cpu->memory[uintConvert(cpu->b, cpu->c)] = cpu->a;
				break;

			// inx b
			case 0x03:
				if (cpu->c < 0xff)
				{
					cpu->c += 1;
				}
				else
				{
					cpu->c = 0;
					cpu->b += 1;
				}
				break;

			// inr b
			case 0x04:
				x = (uint16_t)cpu->b + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->b = (uint8_t)x;
				break;

			// dcr b
			case 0x05:
				x = (uint16_t)cpu->b - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->b = (uint8_t)x;
				break;

			// mvi b, x
			case 0x06:
				cpu->b = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// rlc
			case 0x07:
				x = (uint16_t)cpu->a << 1;
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				break;

			// dad b
			case 0x09:
				x = (uint16_t)cpu->b;
				x = x << 8;
				x = x | (uint16_t)cpu->c;

				y = (uint16_t)cpu->h;
				y = y << 8;
				y = y | (uint16_t)cpu->l;
				z = x + y;
				carryFlag16(cpu, z);
				cpu->l = (uint8_t)(z);
				cpu->h = (uint8_t)(z >> 8);
				break;

			// ldax b
			case 0x0a:
				cpu->a = (uint8_t)cpu->memory[uintConvert(cpu->b, cpu->c)];
				break;

			// dcx b
			case 0x0b:
				if (cpu->c > 0x00)
				{
					cpu->c -= 1;
				}
				else
				{
					cpu->c = 0xff;
					cpu->b -= 1;
				}
				break;

			// inr c
			case 0x0c:
				x = (uint16_t)cpu->c + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->c = (uint8_t)x;
				break;

			// dcr c
			case 0x0d:
				x = (uint16_t)cpu->c - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->c = (uint8_t)x;
				break;

			// mvi c, x
			case 0x0e:
				cpu->c = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// rrc
			case 0x0f:
				x = (uint16_t)cpu->a >> 1;
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x; 
				break;


			// lxi d, x
			case 0x11:
				cpu->d = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// stax d
			case 0x12:
				cpu->memory[uintConvert(cpu->d, cpu->e)] = cpu->a;
				break;

			// inx d
			case 0x13:
				if (cpu->e < 0xff)
				{
					cpu->e += 1;
				}
				else
				{
					cpu->e = 0;
					cpu->d += 1;
				}
				break;

			// inr d
			case 0x14:
				x = (uint16_t)cpu->d + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->d = (uint8_t)x;
				break;

			// dcr d
			case 0x15:
				x = (uint16_t)cpu->d - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->d = (uint8_t)x;
				break;

			// mvi d, x
			case 0x16:
				cpu->d = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// ral
			case 0x17:
				x0 = cpu->a << 1;
				x = x | 0x80;
				x = x & (uint8_t)cpu->carry;
				cpu->carry = (bool)(x >> 8);
				break;

			// dad d
			case 0x19:
				x = (uint16_t)cpu->d;
				x = x << 8;
				x = x | (uint16_t)cpu->e;

				y = (uint16_t)cpu->h;
				y = y << 8;
				y = y | (uint16_t)cpu->l;
				z = x + y;
				carryFlag16(cpu, z);
				cpu->l = (uint8_t)(z);
				cpu->h = (uint8_t)(z >> 8);
				break;

			// ldax d
			case 0x1a:
				cpu->a = (uint8_t)cpu->memory[uintConvert(cpu->d, cpu->e)];
				break;

			// dcx d
			case 0x1b:
				if (cpu->e > 0x00)
				{
					cpu->e -= 1;
				}
				else
				{
					cpu->e = 0xff;
					cpu->d -= 1;
				}
				break;

			// inr e
			case 0x1c:
				x = (uint16_t)cpu->e + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->e = (uint8_t)x;
				break;

			// dcr e
			case 0x1d:
				x = (uint16_t)cpu->e - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->e = (uint8_t)x;
				break;

			// mvi e, x
			case 0x1e:
				cpu->e = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// rar
			case 0x1f:
				x0 = cpu->a >> 1;
				x = x | 0x80;
				x = x & (uint8_t)cpu->carry;
				cpu->carry = (bool)(x >> 8);
				break;


			// lxi h x
			case 0x21:
				cpu->h = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// shld xx
			case 0x22:
				x = (uint16_t)cpu->h;
				x = x << 8;
				x = x | (uint16_t)cpu->l;
				cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] = x;
				cpu->pc += 2;
				break;

			// inx h
			case 0x23:
				if (cpu->l < 0xff)
				{
					cpu->l += 1;
				}
				else
				{
					cpu->l = 0;
					cpu->h += 1;
				}
				break;

			// inr h
			case 0x24:
				x = (uint16_t)cpu->h + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->h = (uint8_t)x;
				break;

			// dcr h
			case 0x25:
				x = (uint16_t)cpu->h - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->h = (uint8_t)x;
				break;

			// mvi h, x
			case 0x26:
				cpu->h = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// daa not implemented for now
			case 0x27:
				break;

			// dad h
			case 0x29:
				x = (uint16_t)cpu->h;
				x = x << 8;
				x = x | (uint16_t)cpu->l;

				y = (uint16_t)cpu->h;
				y = y << 8;
				y = y | (uint16_t)cpu->l;
				z = x + y;
				carryFlag16(cpu, z);
				cpu->l = (uint8_t)(z);
				cpu->h = (uint8_t)(z >> 8);
				break;

			// lhld xx
			case 0x2a:
				//cpu->a = (uint8_t)cpu->memory[uintConvert(cpu->d, cpu->e)];
				cpu->h = cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)];
				cpu->l = cpu->memory[(uintConvert(cpu->pc + 1, cpu->pc + 2)) + 8];
				cpu->pc += 2;
				break;

			// dcx h
			case 0x2b:
				if (cpu->l > 0x00)
				{
					cpu->l -= 1;
				}
				else
				{
					cpu->l = 0xff;
					cpu->h -= 1;
				}
				break;

			// inr l
			case 0x2c:
				x = (uint16_t)cpu->l + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->l = (uint8_t)x;
				break;

			// dcr l
			case 0x2d:
				x = (uint16_t)cpu->l - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->l = (uint8_t)x;
				break;

			// mvi l, x
			case 0x2e:
				cpu->l = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// cma
			case 0x2f:
				cpu->a = ~(cpu->a);
				break;

			// lxi sp x
			case 0x31:
				cpu->sp = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// sta
			case 0x32:
				cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] = cpu->a;
				cpu->pc += 2;
				break;

			// inx sp
			case 0x33:
				cpu->sp += 1;
				break;

			// inr m
			case 0x34:
				x = cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] = x;
				break;

			// dcr m
			case 0x35:
				x = cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] = x;
				break;

			// mvi m, x
			case 0x36:
				cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)] = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// stc
			case 0x37:
				cpu->carry = true;

			// dad sp
			case 0x39:
				x = cpu->sp;

				y = (uint16_t)cpu->h;
				y = y << 8;
				y = y | (uint16_t)cpu->l;
				z = x + y;
				carryFlag16(cpu, z);
				cpu->l = (uint8_t)(z);
				cpu->h = (uint8_t)(z >> 8);
				break;

			// lda xx
			case 0x3a:
				//cpu->a = (uint8_t)cpu->memory[uintConvert(cpu->d, cpu->e)];
				cpu->a = cpu->memory[uintConvert(cpu->pc + 1, cpu->pc + 2)];
				//cpu->l = cpu->memory[(uintConvert(cpu->pc + 1, cpu->pc + 2)) + 8];
				cpu->pc += 2;
				break;

			// dcx sp
			case 0x3b:
				cpu->sp = cpu->sp - 1;
				break;

			// inr a
			case 0x3c:
				x = (uint16_t)cpu->a + 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				break;

			// dcr a
			case 0x3d:
				x = (uint16_t)cpu->a - 1;
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				break;

			// mvi a, x
			case 0x3e:
				cpu->a = cpu->memory[cpu->pc + 1];
				cpu->pc += 1;
				break;

			// cma
			case 0x3f:
				cpu->carry = ~(cpu->carry);
				break;


			// mov b, b
			case 0x40:
				cpu->b = cpu->b;
				break;
				
			// mov b, c
			case 0x41:
				cpu->b = cpu->c;
				break;
				
			// mov b, d
			case 0x42:
				cpu->b = cpu->d;
				break;
				
			// mov b, e
			case 0x43:
				cpu->b = cpu->e;
				break;
				
			// mov b, h
			case 0x44:
				cpu->b = cpu->h;
				break;
				
			// mov b, l
			case 0x45:
				cpu->b = cpu->l;
				break;
				
			// mov b, m
			case 0x46:
				cpu->b = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov b, a
			case 0x47:
				cpu->b = cpu->a;
				break;
				
			// mov c, b
			case 0x48:
				cpu->c = cpu->b;
				break;
				
			// mov c, c
			case 0x49:
				cpu->c = cpu->c;
				break;
				
			// mov c, d
			case 0x4a:
				cpu->c = cpu->d;
				break;
				
			// mov c, e
			case 0x4b:
				cpu->c = cpu->e;
				break;
				
			// mov c, h
			case 0x4c:
				cpu->c = cpu->h;
				break;
				
			// mov c, l
			case 0x4d:
				cpu->c = cpu->l;
				break;
				
			// mov c, m
			case 0x4e:
				cpu->c = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov c, a
			case 0x4f:
				cpu->c = cpu->a;
				break;
				
			// mov d, b
			case 0x50:
				cpu->d = cpu->b;
				break;
				
			// mov d, c
			case 0x51:
				cpu->d = cpu->c;
				break;
				
			// mov d, d
			case 0x52:
				cpu->d = cpu->d;
				break;
				
			// mov d, e
			case 0x53:
				cpu->d = cpu->e;
				break;
				
			// mov d, h
			case 0x54:
				cpu->d = cpu->h;
				break;
				
			// mov d, l
			case 0x55:
				cpu->d = cpu->l;
				break;
				
			// mov d, m
			case 0x56:
				cpu->d = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov d, a
			case 0x57:
				cpu->d = cpu->a;
				break;
				
			// mov e, b
			case 0x58:
				cpu->e = cpu->b;
				break;
				
			// mov e, c
			case 0x59:
				cpu->e = cpu->c;
				break;
				
			// mov e, d
			case 0x5a:
				cpu->e = cpu->d;
				break;
				
			// mov e, e
			case 0x5b:
				cpu->e = cpu->e;
				break;
				
			// mov e, h
			case 0x5c:
				cpu->e = cpu->h;
				break;
				
			// mov e, l
			case 0x5d:
				cpu->e = cpu->l;
				break;
				
			// mov e, m
			case 0x5e:
				cpu->e = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov e, a
			case 0x5f:
				cpu->e = cpu->a;
				break;
				
			// mov h, b
			case 0x60:
				cpu->h = cpu->b;
				break;
				
			// mov h, c
			case 0x61:
				cpu->h = cpu->c;
				break;
				
			// mov h, d
			case 0x62:
				cpu->h = cpu->d;
				break;
				
			// mov h, e
			case 0x63:
				cpu->h = cpu->e;
				break;
				
			// mov h, h
			case 0x64:
				cpu->h = cpu->h;
				break;
				
			// mov h, l
			case 0x65:
				cpu->h = cpu->l;
				break;
				
			// mov h, m
			case 0x66:
				cpu->h = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov h, a
			case 0x67:
				cpu->h = cpu->a;
				break;
				
			// mov l, b
			case 0x68:
				cpu->l = cpu->b;
				break;
				
			// mov l, c
			case 0x69:
				cpu->l = cpu->c;
				break;
				
			// mov l, d
			case 0x6a:
				cpu->l = cpu->d;
				break;
				
			// mov l, e
			case 0x6b:
				cpu->l = cpu->e;
				break;
				
			// mov l, h
			case 0x6c:
				cpu->l = cpu->h;
				break;
				
			// mov l, l
			case 0x6d:
				cpu->l = cpu->l;
				break;
				
			// mov l, m
			case 0x6e:
				cpu->l = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov l, a
			case 0x6f:
				cpu->l = cpu->a;
				break;
				
			// mov m, b
			case 0x70:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->b;
				break;
				
			// mov m, c
			case 0x71:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->c;
				break;
				
			// mov m, d
			case 0x72:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->d;
				break;
				
			// mov m, e
			case 0x73:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->e;
				break;
				
			// mov m, h
			case 0x74:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->h;
				break;
				
			// mov m, l
			case 0x75:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->l;
				break;
				
			// hlt
			case 0x76:
				// for now, I'm just having it be a NOP
				break;				

			// mov m, a
			case 0x77:
				cpu->memory[uintConvert(cpu->h, cpu->l)] = cpu->a;
				break;
				
			// mov a, b
			case 0x78:
				cpu->a = cpu->b;
				break;
				
			// mov a, c
			case 0x79:
				cpu->a = cpu->c;
				break;
				
			// mov a, d
			case 0x7a:
				cpu->a = cpu->d;
				break;
				
			// mov a, e
			case 0x7b:
				cpu->a = cpu->e;
				break;
				
			// mov a, h
			case 0x7c:
				cpu->a = cpu->h;
				break;
				
			// mov a, l
			case 0x7d:
				cpu->a = cpu->l;
				break;
				
			// mov a, m
			case 0x7e:
				cpu->a = cpu->memory[uintConvert(cpu->h, cpu->l)];
				break;
				
			// mov a, a
			case 0x7f:
				cpu->a = cpu->a;
				break;
				
			// add b
			case 0x80:
				add(cpu, cpu->b);
				//cpu->				
				break;
				//a = cpu->a + cpu->b;

			// add c
			case 0x81:
				add(cpu, cpu->c);
				//cpu->				
				break;
				//a = cpu->a + cpu->c;

			// add d
			case 0x82:
				add(cpu, cpu->d);
				//cpu->				
				break;
				//a = cpu->a + cpu->d;

			// add e
			case 0x83:
				add(cpu, cpu->e);
				//cpu->				
				break;
				//a = cpu->a + cpu->e;

			// add h
			case 0x84:
				add(cpu, cpu->h);
				//cpu->				
				break;
				//a = cpu->a + cpu->h;

			// add l
			case 0x85:
				add(cpu, cpu->l);
				//cpu->				
				break;
				//a = cpu->a + cpu->l;

			// add m
			case 0x86:
				add(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// add a
			case 0x87:
				add(cpu, cpu->a);
				//cpu->				
				break;
				//a = cpu->a + cpu->a;

			// adc b
			case 0x88:
				adc(cpu, cpu->b);
				//cpu->				
				break;
				//a = cpu->a + cpu->b + (int)cpu->carry;

			// adc c
			case 0x89:
				adc(cpu, cpu->c);
				//cpu->				
				break;
				//a = cpu->a + cpu->c + (int)cpu->carry;

			// adc d
			case 0x8a:
				adc(cpu, cpu->d);
				//cpu->				
				break;
				//a = cpu->a + cpu->d + (int)cpu->carry;

			// adc e
			case 0x8b:
				adc(cpu, cpu->e);
				//cpu->				
				break;
				//a = cpu->a + cpu->e + (int)cpu->carry;

			// adc h
			case 0x8c:
				adc(cpu, cpu->h);
				//cpu->				
				break;
				//a = cpu->a + cpu->h + (int)cpu->carry;

			// adc l
			case 0x8d:
				adc(cpu, cpu->l);
				//cpu->				
				break;
				//a = cpu->a + cpu->l + (int)cpu->carry;

			// adc m
			case 0x8e:
				adc(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// adc a
			case 0x8f:
				adc(cpu, cpu->a);
				//cpu->				
				break;
				//a = cpu->a + cpu->a + (int)cpu->carry;

			// sub b
			case 0x90:
				sub(cpu, cpu->b);
				//cpu->				
				break;
				//a = cpu->a - cpu->b;

			// sub c
			case 0x91:
				sub(cpu, cpu->c);
				//cpu->				
				break;
				//a = cpu->a - cpu->c;

			// sub d
			case 0x92:
				sub(cpu, cpu->d);
				//cpu->				
				break;
				//a = cpu->a - cpu->d;

			// sub e
			case 0x93:
				sub(cpu, cpu->e);
				//cpu->				
				break;
				//a = cpu->a - cpu->e;

			// sub h
			case 0x94:
				sub(cpu, cpu->h);
				//cpu->				
				break;
				//a = cpu->a - cpu->h;

			// sub l
			case 0x95:
				sub(cpu, cpu->l);
				//cpu->				
				break;
				//a = cpu->a - cpu->l;

			// sub m
			case 0x96:
				sub(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// sub a
			case 0x97:
				sub(cpu, cpu->a);
				//cpu->				
				break;
				//a = cpu->a - cpu->a;

			// sbb b
			case 0x98:
				sbb(cpu, cpu->b);
				//cpu->				
				break;
				//a = cpu->a - (cpu->b + ((int)cpu->carry));

			// sbb c
			case 0x99:
				sbb(cpu, cpu->c);
				//cpu->				
				break;
				//a = cpu->a - (cpu->c + ((int)cpu->carry));

			// sbb d
			case 0x9a:
				sbb(cpu, cpu->d);
				//cpu->				
				break;
				//a = cpu->a - (cpu->d + ((int)cpu->carry));

			// sbb e
			case 0x9b:
				sbb(cpu, cpu->e);
				//cpu->				
				break;
				//a = cpu->a - (cpu->e + ((int)cpu->carry));

			// sbb h
			case 0x9c:
				sbb(cpu, cpu->h);
				//cpu->				
				break;
				//a = cpu->a - (cpu->h + ((int)cpu->carry));

			// sbb l
			case 0x9d:
				sbb(cpu, cpu->l);
				//cpu->				
				break;
				//a = cpu->a - (cpu->l + ((int)cpu->carry));

			// sbb m
			case 0x9e:
				sbb(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// sbb a
			case 0x9f:
				sbb(cpu, cpu->a);
				//cpu->				
				break;
				//a = cpu->a - (cpu->a + ((int)cpu->carry));

			// ana b
			case 0xa0:
				and(cpu, cpu->b);
				//cpu->				
				break;
				//a = cpu->a & cpu->b;

			// ana c
			case 0xa1:
				and(cpu, cpu->c);
				//cpu->				
				break;
				//a = cpu->a & cpu->b;

			// ana d
			case 0xa2:
				and(cpu, cpu->d);
				//cpu->				
				break;
				//a = cpu->a & cpu->c;

			// ana e
			case 0xa3:
				and(cpu, cpu->e);
				//cpu->				
				break;
				//a = cpu->a & cpu->d;

			// ana h
			case 0xa4:
				and(cpu, cpu->h);
			//	cpu->a 				
				break;
				//= cpu->a & cpu->b;

			// ana l
			case 0xa5:
				and(cpu, cpu->l);
				//cpu->				
				break;
				//a = cpu->a & cpu->b;

			// ana m
			case 0xa6:
				and(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// ana a
			case 0xa7:
				and(cpu, cpu->a);
				//cpu->				
				break;
				
			// xra b
			case 0xa8:
				xor(cpu, cpu->b);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->b + ((int)cpu->carry));

			// xra c
			case 0xa9:
				xor(cpu, cpu->c);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->c + ((int)cpu->carry));

			// xra d
			case 0xaa:
				xor(cpu, cpu->d);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->d + ((int)cpu->carry));

			// xra e
			case 0xab:
				xor(cpu, cpu->e);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->e + ((int)cpu->carry));

			// xra h
			case 0xac:
				xor(cpu, cpu->h);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->h + ((int)cpu->carry));

			// xra l
			case 0xad:
				xor(cpu, cpu->l);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->l + ((int)cpu->carry));

			// xra m
			case 0xae:
				xor(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// xra a
			case 0xaf:
				xor(cpu, cpu->a);
				//cpu->
				break;
				//a = cpu->a ^ (cpu->a + ((int)cpu->carry));

			// ora b
			case 0xb0:
				or(cpu, cpu->b);
				break;
				
			// ora c
			case 0xb1:
				or(cpu, cpu->c);
				break;
				
			// ora d
			case 0xb2:
				or(cpu, cpu->d);
				break;
				
			// ora e
			case 0xb3:
				or(cpu, cpu->e);
				break;
				
			// ora h
			case 0xb4:
				or(cpu, cpu->h);
				break;
				
			// ora l
			case 0xb5:
				or(cpu, cpu->l);
				break;
				
			// ora m
			case 0xb6:
				or(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// ora a
			case 0xb7:
				or(cpu, cpu->a);
				break;
				
			// cmp b
			case 0xb8:
				cmp(cpu, cpu->a, cpu->b);
				break;
				
			// cmp c
			case 0xb9:
				cmp(cpu, cpu->a, cpu->c);
				break;
				
			// cmp d
			case 0xba:
				cmp(cpu, cpu->a, cpu->d);
				break;
				
			// cmp e
			case 0xbb:
				cmp(cpu, cpu->a, cpu->e);
				break;
				
			// cmp h
			case 0xbc:
				cmp(cpu, cpu->a, cpu->h);
				break;
				
			// cmp l
			case 0xbd:
				cmp(cpu, cpu->a, cpu->l);
				break;
				
			// cmp m
			case 0xbe:
				cmp(cpu, cpu->a, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// cmp a
			case 0xbf:
				cmp(cpu, cpu->a, cpu->a);
				break;
			
			// rnz	
			case 0xc0:
				if (cpu->zero == 0)
				{
					ret(cpu);
				}
				break;

			// pop b
			case 0xc1:
				pop(cpu, &cpu->b, &cpu->c);
				break;

			// jnz xx
			case 0xc2:
				if (cpu->zero == 0)
				{
					jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				}
				break;

			// jmp xx
			case 0xc3:
				jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				break;

			// cnz xx
			case 0xc4:
				if (cpu->zero == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				break;

			// push b
			case 0xc5:
				push(cpu, (uint16_t)(uintConvert(cpu->b, cpu->c)));
				break;

			// adi n
			case 0xc6:
				x = (uint16_t)cpu->a + cpu->memory[cpu->sp + 1];
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				cpu->pc += 1;
				break;

			// rst 0
			case 0xc7:
				rst(cpu, (uint8_t)0x0000);
				break;

			// rz
			case 0xc8:
				if (cpu->zero == 1)
				{
					ret(cpu); 
				}
				break;

			// ret
			case 0xc9:
				ret(cpu);
				break;

			// jz xx
			case 0xca:
				if (cpu->zero == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;

			// cz xx
			case 0xcc:
				if (cpu->zero == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;

			// cal xx
			case 0xcd:
				call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				break;

			// aci x
			case 0xce:
				x = (uint16_t)cpu->a + cpu->memory[cpu->pc + 1] + (uint8_t)cpu->carry;
				carryFlag(cpu, x);
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				break;


			// rst 1
			case 0xcf:
				rst(cpu, 0x8);
				break;


			// rnc	
			case 0xd0:
				if (cpu->carry == 0)
				{
					ret(cpu);
				}
				break;

			// pop d
			case 0xd1:
				pop(cpu, &cpu->d, &cpu->e);
				break;

			// jnc xx
			case 0xd2:
				if (cpu->carry == 0)
				{
					jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				}
				break;

			// out x
			case 0xd3:
				// instruction not implemented yet
				break;

			// cnc xx
			case 0xd4:
				if (cpu->carry == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				break;

			// push d
			case 0xd5:
				push(cpu, (uint16_t)(uintConvert(cpu->d, cpu->e)));
				break;

			// sui x
			case 0xd6:
				x = (uint16_t)cpu->a - cpu->memory[cpu->sp + 1];
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				cpu->pc += 1;
				break;

			// rst 2
			case 0xd7:
				rst(cpu, (uint8_t)0x0010);
				break;

			// rc
			case 0xd8:
				if (cpu->carry == 1)
				{
					ret(cpu); 
				}
				break;

			// jc xx
			case 0xda:
				if (cpu->zero == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;

			// in x
			case 0xdb:
				// instruction not implemented, so essentially just a NOP
				break;

			// cc xx
			case 0xdc:
				if (cpu->carry == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;


			// sbi x
			case 0xde:
				x = (uint16_t)cpu->a - (cpu->memory[cpu->pc + 1] + (uint8_t)cpu->carry);
				carryFlag(cpu, x);
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				break;

			// exit				
			case 0xdd:
				x = cpu->memory[cpu->pc + 1];
				switch (x)
				{
					case 0x66:	
						running = 1;
						resetExecution(cpu);
						printf("enjoyr your slay 0x%x\n", cpu->sp);
						break;
				}
				break;



			// rst 3
			case 0xdf:
				rst(cpu, 0x18);
				break;


			// rpo	
			case 0xe0:
				if (cpu->parity == 0)
				{
					ret(cpu);
				}
				break;

			// pop h
			case 0xe1:
				pop(cpu, &cpu->h, &cpu->l);
				break;

			// jpo xx
			case 0xe2:
				if (cpu->parity == 0)
				{
					jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				}
				break;

			// xthl
			case 0xe3:
				// instruction not implemented yet
				break;

			// cpo xx
			case 0xe4:
				if (cpu->parity == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				break;

			// push h
			case 0xe5:
				push(cpu, (uint16_t)(uintConvert(cpu->h, cpu->l)));
				break;

			// ani x
			case 0xe6:
				x = (uint16_t)cpu->a & cpu->memory[cpu->sp + 1];
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				cpu->pc += 1;
				break;

			// rst 4
			case 0xe7:
				rst(cpu, (uint8_t)0x0020);
				break;

			// rpe
			case 0xe8:
				if (cpu->parity == 1)
				{
					ret(cpu); 
				}
				break;

			// pchl
			case 0xe9:
				// instruction not implemented yet
				break;

			// jpe xx
			case 0xea:
				if (cpu->parity == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;

			// in x
			case 0xeb:
				// instruction not implemented, so essentially just a NOP
				break;

			// cpe xx
			case 0xec:
				if (cpu->parity == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;


			// xri x
			case 0xee:
				x = (uint16_t)cpu->a ^ cpu->memory[cpu->pc + 1];
				carryFlag(cpu, x);
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				break;


			// rst 5
			case 0xef:
				rst(cpu, 0x28);
				break;

			// rp
			case 0xf0:
				if (cpu->sign == 0)
				{
					ret(cpu);
				}
				break;

			// pop psw
			case 0xf1:
				// instruction not implemented yet NOP
				break;

			// jp xx
			case 0xf2:
				if (cpu->sign == 0)
				{
					jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				}
				break;

			// di
			case 0xf3:
				// instruction not implemented yet
				break;

			// cp xx
			case 0xf4:
				if (cpu->sign == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				break;

			// push psw
			case 0xf5:
				// instruction not implemented yet
				break;

			// ori x
			case 0xf6:
				x = (uint16_t)cpu->a | cpu->memory[cpu->sp + 1];
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				carryFlag(cpu, x);
				cpu->a = (uint8_t)x;
				cpu->pc += 1;
				break;

			// rst 6
			case 0xf7:
				rst(cpu, (uint8_t)0x0030);
				break;

			// rm
			case 0xf8:
				if (cpu->sign == 1)
				{
					ret(cpu); 
				}
				break;

			// sphl
			case 0xf9:
				// instruction not implemented yet
				break;

			// jm xx
			case 0xfa:
				if (cpu->sign == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;

			// ei
			case 0xfb:
				// instruction not implemented, so essentially just a NOP
				break;

			// cm xx
			case 0xfc:
				if (cpu->sign == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				break;


			// cpi x
			case 0xfe:
				x = (uint16_t)cpu->a - cpu->memory[cpu->pc + 1];
				carryFlag(cpu, x);
				parityFlag(cpu, x);
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				break;


			// rst 5
			case 0xff:
				rst(cpu, 0x28);
				break;
		}
	}
}

/*
7:	Sign (S Flag)	
6:	Zero (Z Flag)
5:	0 (It always holds the value 0)
4:	Auxillary (AC or H Flag)
3:	0 (It always holds the value 0)	
2:	Parity (P Flag)	
1:	1 (It always holds the value 1)
0:	Carry (C Flag)
*/
uint8_t convertFlagsRegister(cpu8080 *cpu)
{
	uint8_t x = 0x2;
	x = x | ((uint8_t)(cpu->sign == true) << 7);
	x = x | ((uint8_t)(cpu->zero == true) << 6);
	x = x | ((uint8_t)(cpu->aux == true) << 4);
	x = x | ((uint8_t)(cpu->parity == true) << 2);
	x = x | ((uint8_t)(cpu->carry == true) << 1);
	return x;
}

void printFlags(cpu8080 *cpu)
{
	puts("\nFlags");
	printf("7: Sign flag: %d\n", (int)cpu->sign);
	printf("6: Zero flag: %d\n", (int)cpu->zero);
	printf("5: zero1 flag (value shouldn't change): %d\n", cpu->zero1);
	printf("4: Auxillary flag: %d\n", (int)cpu->aux);
	printf("3: zero2 flag (value shouldn't change): %d\n", cpu->zero2);
	printf("2: Parity flag: %d\n", (int)cpu->parity);
	printf("1: one flag (value shouldn't change): %d\n", cpu->one);
	printf("0: Carry flag: %d\n", (int)cpu->carry);
}

void printRegisters(cpu8080 *cpu)
{
	puts("8 bit registers");
	printf("register a: 0x%x\n", (int)cpu->a);
	printf("register b: 0x%x\n", (int)cpu->b);
	printf("register c: 0x%x\n", (int)cpu->c);
	printf("register d: 0x%x\n", (int)cpu->d);
	printf("register e: 0x%x\n", (int)cpu->e);
	printf("register h: 0x%x\n", (int)cpu->h);
	printf("register l: 0x%x\n", (int)cpu->l);

	puts("\n16 bit registers");
	printf("register bc: 0x%x\n", (int)uintConvert(cpu->b, cpu->c));
	printf("register de: 0x%x\n", (int)uintConvert(cpu->d, cpu->e));
	printf("register hl: 0x%x\n", (int)uintConvert(cpu->h, cpu->l));
	printf("register af: 0x%x\n", (int)uintConvert(cpu->a, convertFlagsRegister(cpu)));

	printFlags(cpu);

	puts("\nStack registers (16 bit)");
	printf("register pc: 0x%x\n", (int)cpu->pc);
	printf("register sp: 0x%x\n", (int)cpu->sp);
	return;
}

char *removeBeginning(char *inp)
{
	int x, i;
	char *result;

	x = strlen(inp);
	result = malloc(x);
	for (i = 2; i < x; i++)
	{
		result[i - 2] = inp[i];
	}
	return result;
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

int charToInt(char * inp)
{
	char *arg;
	int x;

	arg = removeBeginning(inp);
	if (strncmp(arg, "0x", 2) == 0)
	{
		puts("base 16");
		x = strtoumax(arg, NULL, 16);

	}
	else
	{
		x = strtoumax(arg, NULL, 10);
	}	
}

void createBreakpoint(cpu8080 *cpu, char *menuInput)
{
	char *arg;
	int x;
	breakpoints *last, *next;
	last = findLastBreakpoint(cpu->breakpoints);

	x = charToInt(menuInput);

	next = allocateBreakpoint((uint16_t) x);
	next->active = true;
	if (last == NULL)
	{
		cpu->breakpoints = next;
	}
	else
	{
		last->next = next;
	}
}

void showBreakpoints(breakpoints *breakpoint, int n)
{
	if (breakpoint == NULL)
	{
		return;
	}
	else
	{
		printf("Breakpoint %x at %x\t\t(active: %d)\n", n, breakpoint->breakp, (int)breakpoint->active);
		return showBreakpoints(breakpoint->next, n + 1);
	}
}

void deactivateBreakpoint(cpu8080 *cpu, char *menuInput)
{
	int i, n;
	breakpoints *b;
	b = cpu->breakpoints;

	if (b == NULL)
	{
		puts("There are no breakpoints.");
		return;
	}

	n = charToInt(menuInput);
	for (i = 0; i < n; i++)
	{
		b = b->next;
		if (b == NULL)
		{
			puts("That breakpoint doesn't exist");
			return;
		}
	}

	b->active = false;
}


void activateBreakpoint(cpu8080 *cpu, char *menuInput)
{
	int i, n;
	breakpoints *b;
	b = cpu->breakpoints;

	if (b == NULL)
	{
		puts("There are no breakpoints.");
		return;
	}

	n = charToInt(menuInput);
	for (i = 0; i < n; i++)
	{
		b = b->next;
		if (b == NULL)
		{
			puts("That breakpoint doesn't exist");
			return;
		}
	}

	b->active = true;
}

void printMemoryRegions(cpu8080 *cpu)
{
	printf("Total Memory Region:\t 0x%x - 0x%x\n", (int)cpu->memBeg, (int)cpu->memEnd);
	printf("Code Memory Region:\t 0x%x - 0x%x\n", (int)cpu->codeBeg, (int)cpu->codeEnd);
	printf("Stack Memory Region:\t 0x%x - 0x%x\n", (int)cpu->stackBeg, (int)cpu->sp);
}

void destroyCpu(cpu8080 *cpu)
{
	free(cpu->memory);
	cpu->memory = 0;
	free(cpu);
	cpu = 0;
	return;
}

void printRegister(cpu8080 *cpu, char *menuInput)
{
	char *arg;

	arg = removeBeginning(menuInput);	

	if (strncmp(arg, "bc", 2) == 0)
	{
		printf("register bc: 0x%x\n", (int)uintConvert(cpu->b, cpu->c));
		return;
	}

	else if (strncmp(arg, "de", 2) == 0)
	{
		printf("register de: 0x%x\n", (int)uintConvert(cpu->b, cpu->c));
		return;
	}	

	else if (strncmp(arg, "hl", 2) == 0)
	{
		printf("register hl: 0x%x\n", (int)uintConvert(cpu->h, cpu->l));
		return;
	}

	else if (strncmp(arg, "af", 2) == 0)
	{
		printf("register af: 0x%x\n", (int)uintConvert(cpu->h, convertFlagsRegister(cpu)));
		return;
	}

	else if (strncmp(arg, "pc", 2) == 0)
	{
		printf("register pc: 0x%x\n", (int)cpu->pc);
		return;
	}

	else if (strncmp(arg, "sp", 2) == 0)
	{
		printf("register sp: 0x%x\n", (int)cpu->sp);
		return;
	}

	switch(arg[0])
	{
		case 'a':
			printf("register a: 0x%x\n", (uint16_t)cpu->a);
			return;
			break;
		case 'b':
			printf("register b: 0x%x\n", (uint16_t)cpu->b);
			return;
			break;
		case 'c':
			printf("register c: 0x%x\n", (uint16_t)cpu->c);
			return;
			break;
		case 'd':
			printf("register d: 0x%x\n", (uint16_t)cpu->d);
			return;
			break;
		case 'e':
			printf("register e: 0x%x\n", (uint16_t)cpu->e);
			return;
			break;
		case 'h':
			printf("register h: 0x%x\n", (uint16_t)cpu->h);
			return;
			break;
		case 'l':
			printf("register l: 0x%x\n", (uint16_t)cpu->l);
			return;
			break;
		case 'f':
			printFlags(cpu);
			return;
			break;
	}
}

void menu(cpu8080 *cpu)
{
	uint16_t breakpoint;

	char *menuChoice;
	menuChoice = malloc(menuInputSize);
	while (1)
	{
		printf("debugger> ");
		fgets(menuChoice, menuInputSize - 1, stdin);
		switch (menuChoice[0])
		{
			case 'r':
				resetExecution(cpu);
				emulate(cpu);
				break;
			case 'i':
				printRegisters(cpu);
				break;
			case 'b':
				createBreakpoint(cpu, menuChoice);
				break;
			case 's':
				showBreakpoints(cpu->breakpoints, 0);
				break;
			case 'c':
				emulate(cpu);
				break;
			case 'd':
				deactivateBreakpoint(cpu, menuChoice);
				break;
			case 'a':
				activateBreakpoint(cpu, menuChoice);
				break;
			case 'm':
				printMemoryRegions(cpu);
				break;
			case 'q':
				resetExecution(cpu);
				destroyCpu(cpu);
				return;
				break;
			case 'p':
				printRegister(cpu, menuChoice);
				break;
		}
	}
}

int main(int argc, char **argv)
{
	cpu8080 *cpu;
	char *binaryCode;

	if (argc != 2)
	{
		puts("gimme input file");
		exit(0);
	}

	cpu = createCpu();

	scanBinary(argv[1], cpu);

	menu(cpu);

	return 0;
}