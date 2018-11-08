#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// This file has the code which actually runs the operations for each opcode

void emulate(cpu8080 *cpu)
{
	// Variables for helping us tell what code we are executing
	int running;
	int stepIns;
	uint8_t op;

	// Variables for helping with calculations
	uint8_t x0;
	uint16_t x;
	uint16_t y;
	uint32_t z;

	// This variable just keeps track if we are running
	running = 1;

	// Run it as a for loop for every possible instruction between 0 - 0xffff (all 16 bits of memory), and while we are running
	for (cpu->pc = cpu->pc; cpu->pc < 0x1000 && running == 1; cpu->pc++)
	{
		// Set the opcode to the next instruction
		op = (uint8_t)cpu->memory[cpu->pc];
		
		// Check if we hit a breakpoint
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

		// See if we are stepping through an instruction, and break accordingly

		if (cpu->step == 1)
		{
			cpu->step = 2;
		}

		else if (cpu->step == 2)
		{
			printf("Stepped to 0x%x\n", (int)cpu->pc);
			cpu->step = 0;
			return;
		}

		// See if we are disassembling the instructions as they are ran
		if (cpu->disasAsExec == true)
		{
			disass(cpu, cpu->pc);
		}

		// Here is a switch statement which maps specific op codes to their respective functionallity

		switch (op)
		{
			// nop
			case 0x00:
				break;

			// lxi b, xx
			case 0x01:
				lxi(cpu, (&cpu->b), (&cpu->c));
				break;

			// stax b
			case 0x02:
				stax(cpu, cpu->b, cpu->c);
				break;

			// inx b
			case 0x03:
				inx8((&cpu->b), (&cpu->c));
				break;

			// inr b
			case 0x04:
				inr(cpu, &cpu->b);
				break;

			// dcr b
			case 0x05:
				dcr(cpu, &(cpu->b));
				break;

			// mvi b, x
			case 0x06:
				mvi(cpu, &(cpu->b), cpu->memory[cpu->pc + 1]);
				break;

			// rlc
			case 0x07:
				x = ((cpu->a & 0x80) >> 7);
				cpu->carry = (x);
				cpu->a = (cpu->a << 1);
				cpu->a = (cpu->a | x);
				break;

			// dad b
			case 0x09:
				dad(cpu, (&cpu->b), (&cpu->c));
				break;

			// ldax b
			case 0x0a:
				ldax(cpu, cpu->b, cpu->c);
				break;

			// dcx b
			case 0x0b:
				dcx8((&cpu->b), (&cpu->c));
				break;

			// inr c
			case 0x0c:
				inr(cpu, &(cpu->c));
				break;

			// dcr c
			case 0x0d:
				dcr(cpu, &(cpu->c));
				break;

			// mvi c, x
			case 0x0e:
				mvi(cpu, &(cpu->c), cpu->memory[cpu->pc + 1]);
				break;


			// rrc
			case 0x0f:
				x = (cpu->a & 0x1);
				cpu->carry = x;
				cpu->a = (cpu->a >> 1);
				cpu->a = (cpu->a | (x << 7));
				break;


			// lxi d, xx
			case 0x11:
				lxi(cpu, (&cpu->d), (&cpu->e));
				break;

			// stax d
			case 0x12:
				stax(cpu, cpu->d, cpu->e);
				break;

			// inx d
			case 0x13:
				inx8((&cpu->d), (&cpu->e));
				break;

			// inr d
			case 0x14:
				inr(cpu, &(cpu->d));
				break;

			// dcr d
			case 0x15:
				dcr(cpu, &(cpu->d));
				break;

			// mvi d, x
			case 0x16:
				mvi(cpu, &(cpu->d), cpu->memory[cpu->pc + 1]);
				break;

			// ral
			case 0x17:
				x = (cpu->a & 0x80) >> 7;
				y = (int)cpu->carry;
				cpu->carry = (x);
				cpu->a = (cpu->a << 1);
				cpu->a = cpu->a & 0xfe;
				cpu->a = cpu->a | (y);
				break;

			// dad d
			case 0x19:
				dad(cpu, (&cpu->d), (&cpu->e));
				break;

			// ldax d
			case 0x1a:
				ldax(cpu, cpu->d, cpu->e);
				break;

			// dcx d
			case 0x1b:
				dcx8((&cpu->d), (&cpu->e));
				break;

			// inr e
			case 0x1c:
				inr(cpu, &(cpu->e));
				break;

			// dcr e
			case 0x1d:
				dcr(cpu, &(cpu->e));
				break;

			// mvi e, x
			case 0x1e:
				mvi(cpu, &(cpu->e), cpu->memory[cpu->pc + 1]);
				break;

			// rar
			case 0x1f:
				x = cpu->a & 0x01;
				y = (int)cpu->carry;
				cpu->carry = x;
				cpu->a = (cpu->a >> 1);
				cpu->a = cpu->a & 0x7f;
				cpu->a = cpu->a | (y << 7);
				break;


			// lxi h, xx
			case 0x21:
				lxi(cpu, (&cpu->h), (&cpu->l));
				break;

			// shld xx
			case 0x22:
				cpu->memory[uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2])] = cpu->l;
				cpu->memory[uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]) + 1] = cpu->h;
				cpu->pc += 2;
				break;

			// inx h
			case 0x23:
				inx8((&cpu->h), (&cpu->l));
				break;

			// inr h
			case 0x24:
				inr(cpu, &(cpu->h));
				break;

			// dcr h
			case 0x25:
				dcr(cpu, &(cpu->h));
				break;

			// mvi h, x
			case 0x26:
				mvi(cpu, &(cpu->h), cpu->memory[cpu->pc + 1]);
				break;

			// daa 
			case 0x27:
				x = cpu->a;
				if ((x & 0xf) > 9 || cpu->aux == true)
				{
					x = cpu->a += 6;
					cpu->a += x;
					auxiliaryFlag(cpu, (cpu->a & 0xf), 6, 1);
				}
				if ((x >> 4) > 9 || cpu->carry == true)
				{
					y = (x >> 4) + 6;
					x = x & 0xf;
					x = x | y;
					cpu->a = x;
					auxiliaryFlag(cpu, (cpu->a & 0xf0), 6, 1);
				}
				zeroFlag(cpu, x);
				signFlag(cpu, x);
				parityFlag(cpu, x);
				carryFlag(cpu, x);
				break;

			// dad h
			case 0x29:
				dad(cpu, (&cpu->h), (&cpu->l));
				break;

			// lhld xx
			case 0x2a:
				cpu->l = cpu->memory[uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2])];
				cpu->h = cpu->memory[(uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2])) + 1];
				cpu->pc += 2;
				break;

			// dcx h
			case 0x2b:
				dcx8((&cpu->h), (&cpu->l));
				break;

			// inr l
			case 0x2c:
				inr(cpu, &(cpu->l));
				break;

			// dcr l
			case 0x2d:
				dcr(cpu, &(cpu->l));
				break;

			// mvi l, x
			case 0x2e:
				mvi(cpu, &(cpu->l), cpu->memory[cpu->pc + 1]);
				break;

			// cma
			case 0x2f:
				cpu->a = ~(cpu->a);
				break;

			// lxi sp xx
			case 0x31:
				lxi16(cpu, (&cpu->sp));
				break;

			// sta xx
			case 0x32:
				cpu->memory[uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2])] = cpu->a;
				cpu->pc += 2;
				break;

			// inx sp
			case 0x33:
				inx16(&cpu->sp);
				break;

			// inr m
			case 0x34:
				inr(cpu, &(cpu->memory[uintConvert(cpu->h, cpu->l)]));
				break;

			// dcr m
			case 0x35:
				dcr(cpu, &(cpu->memory[uintConvert(cpu->h, cpu->l)]));
				break;

			// mvi m, x
			case 0x36:
				mvi(cpu, &(cpu->memory[uintConvert(cpu->h, cpu->l)]), cpu->memory[cpu->pc + 1]);
				break;

			// stc
			case 0x37:
				cpu->carry = true;

			// dad sp
			case 0x39:
				dad16(cpu, (&cpu->sp));
				break;

			// lda xx
			case 0x3a:
				cpu->a = cpu->memory[uintConvert(cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2])];
				cpu->pc += 2;
				break;

			// dcx sp
			case 0x3b:
				dcx16(&cpu->sp);
				break;

			// inr a
			case 0x3c:
				inr(cpu, &(cpu->a));
				break;

			// dcr a
			case 0x3d:
				dcr(cpu, &(cpu->a));
				break;

			// mvi a, x
			case 0x3e:
				mvi(cpu, &(cpu->a), cpu->memory[cpu->pc + 1]);
				break;

			// cmc
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
				
			// hlt I don't have interrupts implemented, so it's just a NOP
			case 0x76:
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
				break;

			// add c
			case 0x81:
				add(cpu, cpu->c);	
				break;

			// add d
			case 0x82:
				add(cpu, cpu->d);		
				break;

			// add e
			case 0x83:
				add(cpu, cpu->e);		
				break;

			// add h
			case 0x84:
				add(cpu, cpu->h);		
				break;

			// add l
			case 0x85:
				add(cpu, cpu->l);	
				break;

			// add m
			case 0x86:
				add(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// add a
			case 0x87:
				add(cpu, cpu->a);
				break;

			// adc b
			case 0x88:
				adc(cpu, cpu->b);		
				break;

			// adc c
			case 0x89:
				adc(cpu, cpu->c);
				break;

			// adc d
			case 0x8a:
				adc(cpu, cpu->d);
				break;

			// adc e
			case 0x8b:
				adc(cpu, cpu->e);
				break;

			// adc h
			case 0x8c:
				adc(cpu, cpu->h);		
				break;

			// adc l
			case 0x8d:
				adc(cpu, cpu->l);	
				break;

			// adc m
			case 0x8e:
				adc(cpu, cpu->memory[(int)uintConvert(cpu->h, cpu->l)]);
				break;
				
			// adc a
			case 0x8f:
				adc(cpu, cpu->a);
				break;

			// sub b
			case 0x90:
				sub(cpu, cpu->b);
				break;

			// sub c
			case 0x91:
				sub(cpu, cpu->c);		
				break;

			// sub d
			case 0x92:
				sub(cpu, cpu->d);
				break;

			// sub e
			case 0x93:
				sub(cpu, cpu->e);		
				break;

			// sub h
			case 0x94:
				sub(cpu, cpu->h);
				break;

			// sub l
			case 0x95:
				sub(cpu, cpu->l);
				break;

			// sub m
			case 0x96:
				sub(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// sub a
			case 0x97:
				sub(cpu, cpu->a);
				break;

			// sbb b
			case 0x98:
				sbb(cpu, cpu->b);				
				break;

			// sbb c
			case 0x99:
				sbb(cpu, cpu->c);		
				break;

			// sbb d
			case 0x9a:
				sbb(cpu, cpu->d);
				break;

			// sbb e
			case 0x9b:
				sbb(cpu, cpu->e);
				break;

			// sbb h
			case 0x9c:
				sbb(cpu, cpu->h);
				break;

			// sbb l
			case 0x9d:
				sbb(cpu, cpu->l);
				break;

			// sbb m
			case 0x9e:
				sbb(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// sbb a
			case 0x9f:
				sbb(cpu, cpu->a);		
				break;

			// ana b
			case 0xa0:
				and(cpu, cpu->b);		
				break;

			// ana c
			case 0xa1:
				and(cpu, cpu->c);				
				break;

			// ana d
			case 0xa2:
				and(cpu, cpu->d);			
				break;

			// ana e
			case 0xa3:
				and(cpu, cpu->e);			
				break;

			// ana h
			case 0xa4:
				and(cpu, cpu->h);
				break;

			// ana l
			case 0xa5:
				and(cpu, cpu->l);
				break;

			// ana m
			case 0xa6:
				and(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// ana a
			case 0xa7:
				and(cpu, cpu->a);
				break;
				
			// xra b
			case 0xa8:
				xor(cpu, cpu->b);
				break;

			// xra c
			case 0xa9:
				xor(cpu, cpu->c);
				break;

			// xra d
			case 0xaa:
				xor(cpu, cpu->d);
				break;

			// xra e
			case 0xab:
				xor(cpu, cpu->e);
				break;

			// xra h
			case 0xac:
				xor(cpu, cpu->h);
				break;

			// xra l
			case 0xad:
				xor(cpu, cpu->l);
				break;

			// xra m
			case 0xae:
				xor(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// xra a
			case 0xaf:
				xor(cpu, cpu->a);
				break;

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
				cmp(cpu, cpu->b);
				break;
				
			// cmp c
			case 0xb9:
				cmp(cpu, cpu->c);
				break;
				
			// cmp d
			case 0xba:
				cmp(cpu, cpu->d);
				break;
				
			// cmp e
			case 0xbb:
				cmp(cpu, cpu->e);
				break;
				
			// cmp h
			case 0xbc:
				cmp(cpu, cpu->h);
				break;
				
			// cmp l
			case 0xbd:
				cmp(cpu, cpu->l);
				break;
				
			// cmp m
			case 0xbe:
				cmp(cpu, cpu->memory[uintConvert(cpu->h, cpu->l)]);
				break;
				
			// cmp a
			case 0xbf:
				cmp(cpu, cpu->a);
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
				else
				{
					cpu->pc += 2;
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
				else
				{
					cpu->pc += 2;
				}
				break;

			// push b
			case 0xc5:
				push(cpu, cpu->b, cpu->c);
				break;

			// adi n
			case 0xc6:
				add(cpu, cpu->memory[cpu->pc + 1]);
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
				else
				{
					cpu->pc += 2;
				}
				break;

			// cz xx
			case 0xcc:
				if (cpu->zero == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// call xx
			case 0xcd:
				call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				break;

			// aci x
			case 0xce:
				adc(cpu, cpu->memory[cpu->pc + 1]);
				cpu->pc += 1;
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
				else
				{
					cpu->pc += 2;
				}
				break;

			// out x, I haven't implemented other hardware devices so this is essentially a NOP with an arg
			case 0xd3:
				cpu->pc += 1;
				break;

			// cnc xx
			case 0xd4:
				if (cpu->carry == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// push d
			case 0xd5:
				push(cpu, cpu->d, cpu->e);
				break;

			// sui x
			case 0xd6:
				sub(cpu, cpu->memory[cpu->pc + 1]);
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
				else
				{
					cpu->pc += 2;
				}
				break;

			// in x, I haven't implemented other hardware devices so this is essentially a NOP with an arg
			case 0xdb:
				cpu->pc += 1;
				break;

			// cc xx
			case 0xdc:
				if (cpu->carry == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;


			// sbi x
			case 0xde:
				sbb(cpu, cpu->memory[cpu->pc + 1]);
				cpu->pc += 1;
				break;

			// End Of Assembly : This insturction is to signify the end, with an arg of 0x66 meaning to end the code		
			case 0xdd:
				x = cpu->memory[cpu->pc + 1];
				switch (x)
				{
					case 0x66:	
						running = 0;
						resetExecution(cpu);
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
				else
				{
					cpu->pc += 2;
				}
				break;

			// xthl
			case 0xe3:
				x = cpu->l;
				y = cpu->h;
				cpu->l = cpu->memory[cpu->sp];
				cpu->h = cpu->memory[cpu->sp + 1];
				cpu->memory[cpu->sp] = x;
				cpu->memory[cpu->sp + 1] = y;
				break;

			// cpo xx
			case 0xe4:
				if (cpu->parity == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// push h
			case 0xe5:
				push(cpu, cpu->h, cpu->l);
				break;

			// ani x
			case 0xe6:
				and(cpu, cpu->memory[cpu->pc + 1]);
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
				x = cpu->h;
				x = (x << 8);
				x = (x | cpu->l);
				cpu->pc = (x - 1);
				break;

			// jpe xx
			case 0xea:
				if (cpu->parity == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// xchg
			case 0xeb:
				x = cpu->d;
				y = cpu->e;
				cpu->d = cpu->h;
				cpu->e = cpu->l;
				cpu->h = x;
				cpu->l = y;
				break;

			// cpe xx
			case 0xec:
				if (cpu->parity == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;


			// xri x
			case 0xee:
				xor(cpu, cpu->memory[cpu->pc + 1]);
				cpu->pc += 1;
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
				popPsw(cpu);
				break;

			// jp xx
			case 0xf2:
				if (cpu->sign == 0)
				{
					jmp(cpu, (uint8_t)cpu->memory[cpu->pc + 1], (uint8_t)cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// di
			case 0xf3:
				cpu->allowInterrupt = 0;
				break;

			// cp xx
			case 0xf4:
				if (cpu->sign == 0)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc  + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// push psw
			case 0xf5:
				push(cpu, cpu->a, convertFlagsRegister(cpu));
				break;

			// ori x
			case 0xf6:
				or(cpu, cpu->memory[cpu->pc + 1]);
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
				x = cpu->h;
				x = (x << 8);
				x = (x | cpu->l);
				cpu->sp = x;
				break;

			// jm xx
			case 0xfa:
				if (cpu->sign == 1)
				{
					jmp(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;

			// ei
			case 0xfb:
				cpu->allowInterrupt = 1;
				break;

			// cm xx
			case 0xfc:
				if (cpu->sign == 1)
				{
					call(cpu, cpu->memory[cpu->pc + 1], cpu->memory[cpu->pc + 2]);
				}
				else
				{
					cpu->pc += 2;
				}
				break;


			// cpi x
			case 0xfe:
				cmp(cpu, cpu->memory[cpu->pc + 1]);
				cpu->pc += 1;
				break;


			// rst 7
			case 0xff:
				rst(cpu, 0x38);
				break;
		}
	}
}