#include <stdbool.h>
#include <inttypes.h>
#include "Nightfall.h"

// This source file deals with disassembly
// It's essentially just a giant switch statement

int disass(cpu8080 *cpu, int i)
{
	switch (cpu->memory[i])
	{
		case 0x00:
			printf("0x%x: nop\n", i);
			return i + 1;

		case 0x01:
			printf("0x%x: lxi b, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x02:
			printf("0x%x: stax b\n", i);
			return i + 1;

		case 0x03:
			printf("0x%x: inx b\n", i);
			return i + 1;

		case 0x04:
			printf("0x%x: inr b\n", i);
			return i + 1;

		case 0x05:
			printf("0x%x: dcr b\n", i);
			return i + 1;

		case 0x06:
			printf("0x%x: mvi b, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x07:
			printf("0x%x: rlc\n", i);
			return i + 1;

		case 0x08:
			printf("0x%x: 0x08 instruction not implemented.\n", i);
			return i + 1;

		case 0x09:
			printf("0x%x: dad b\n", i);
			return i + 1;

		case 0x0a:
			printf("0x%x: ldax b\n", i);
			return i + 1;

		case 0x0b:
			printf("0x%x: dcx b\n", i);
			return i + 1;

		case 0x0c:
			printf("0x%x: inr c\n", i);
			return i + 1;

		case 0x0d:
			printf("0x%x: dcr c\n", i);
			return i + 1;

		case 0x0e:
			printf("0x%x: mvi c, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x0f:
			printf("0x%x: rrc\n", i);
			return i + 1;

		case 0x10:
			printf("0x%x: 0x10 instruction not implemented.\n", i);
			return i + 1;

		case 0x11:
			printf("0x%x: lxi d, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x12:
			printf("0x%x: stax d\n", i);
			return i + 1;

		case 0x13:
			printf("0x%x: inx d\n", i);
			return i + 1;

		case 0x14:
			printf("0x%x: inr d\n", i);
			return i + 1;

		case 0x15:
			printf("0x%x: dcr d\n", i);
			return i + 1;

		case 0x16:
			printf("0x%x: mvi d, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x17:
			printf("0x%x: ral\n", i);
			return i + 1;
			
		case 0x18:
			printf("0x%x: 0x18 instruction not implemented.\n", i);
			return i + 1;

		case 0x19:
			printf("0x%x: dad d\n", i);
			return i + 1;

		case 0x1a:
			printf("0x%x: ldax d\n", i);
			return i + 1;

		case 0x1b:
			printf("0x%x: dcx d\n", i);
			return i + 1;

		case 0x1c:
			printf("0x%x: inr e\n", i);
			return i + 1;

		case 0x1d:
			printf("0x%x: dcr e\n", i);
			return i + 1;

		case 0x1e:
			printf("0x%x: mvi e, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x1f:
			printf("0x%x: rar\n", i);
			return i + 1;

		case 0x20:
			printf("0x%x: 0x20 instruction not implemented.\n", i);
			return i + 1;

		case 0x21:
			printf("0x%x: lxi h, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x22:
			printf("0x%x: shld, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x23:
			printf("0x%x: inx h\n", i);
			return i + 1;

		case 0x24:
			printf("0x%x: inr h\n", i);
			return i + 1;

		case 0x25:
			printf("0x%x: dcr h\n", i);
			return i + 1;

		case 0x26:
			printf("0x%x: mvi h, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x27:
			printf("0x%x: daa\n", i);
			return i + 1;
			
		case 0x28:
			printf("0x%x: 0x28 instruction not implemented.\n", i);
			return i + 1;

		case 0x29:
			printf("0x%x: dad h\n", i);
			return i + 1;

		case 0x2a:
			printf("0x%x: lhld, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x2b:
			printf("0x%x: dcx h\n", i);
			return i + 1;

		case 0x2c:
			printf("0x%x: inr l\n", i);
			return i + 1;

		case 0x2d:
			printf("0x%x: dcr l\n", i);
			return i + 1;

		case 0x2e:
			printf("0x%x: mvi l, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x2f:
			printf("0x%x: cma\n", i);
			return i + 1;

		case 0x30:
			printf("0x%x: 0x30 instruction not implemented.\n", i);
			return i + 1;

		case 0x31:
			printf("0x%x: lxi sp, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x32:
			printf("0x%x: sta, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x33:
			printf("0x%x: inx sp\n", i);
			return i + 1;

		case 0x34:
			printf("0x%x: inr m\n", i);
			return i + 1;

		case 0x35:
			printf("0x%x: dcr m\n", i);
			return i + 1;

		case 0x36:
			printf("0x%x: mvi m, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x37:
			printf("0x%x: stc\n", i);
			return i + 1;
			
		case 0x38:
			printf("0x%x: 0x38 instruction not implemented.\n", i);
			return i + 1;

		case 0x39:
			printf("0x%x: dad sp\n", i);
			return i + 1;

		case 0x3a:
			printf("0x%x: lda, ", i);
			printTwoArgs(cpu, i);
			return i + 3;

		case 0x3b:
			printf("0x%x: dcx sp\n", i);
			return i + 1;

		case 0x3c:
			printf("0x%x: inr a\n", i);
			return i + 1;

		case 0x3d:
			printf("0x%x: dcr a\n", i);
			return i + 1;

		case 0x3e:
			printf("0x%x: mvi a, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0x3f:
			printf("0x%x: cmc\n", i);
			return i + 1;


		case 0x40:
			printf("0x%x: mov b, b\n", i);
			return i + 1;

		case 0x41:
			printf("0x%x: mov b, c\n", i);
			return i + 1;

		case 0x42:
			printf("0x%x: mov b, d\n", i);
			return i + 1;

		case 0x43:
			printf("0x%x: mov b, e\n", i);
			return i + 1;

		case 0x44:
			printf("0x%x: mov b, h\n", i);
			return i + 1;

		case 0x45:
			printf("0x%x: mov b, l\n", i);
			return i + 1;

		case 0x46:
			printf("0x%x: mov b, m\n", i);
			return i + 1;

		case 0x47:
			printf("0x%x: mov b, a\n", i);
			return i + 1;

		case 0x48:
			printf("0x%x: mov c, b\n", i);
			return i + 1;

		case 0x49:
			printf("0x%x: mov c, c\n", i);
			return i + 1;

		case 0x4a:
			printf("0x%x: mov c, d\n", i);
			return i + 1;

		case 0x4b:
			printf("0x%x: mov c, e\n", i);
			return i + 1;

		case 0x4c:
			printf("0x%x: mov c, h\n", i);
			return i + 1;

		case 0x4d:
			printf("0x%x: mov c, l\n", i);
			return i + 1;

		case 0x4e:
			printf("0x%x: mov c, m\n", i);
			return i + 1;

		case 0x4f:
			printf("0x%x: mov c, a\n", i);
			return i + 1;

		case 0x50:
			printf("0x%x: mov d, b\n", i);
			return i + 1;

		case 0x51:
			printf("0x%x: mov d, c\n", i);
			return i + 1;

		case 0x52:
			printf("0x%x: mov d, d\n", i);
			return i + 1;

		case 0x53:
			printf("0x%x: mov d, e\n", i);
			return i + 1;

		case 0x54:
			printf("0x%x: mov d, h\n", i);
			return i + 1;

		case 0x55:
			printf("0x%x: mov d, l\n", i);
			return i + 1;

		case 0x56:
			printf("0x%x: mov d, m\n", i);
			return i + 1;

		case 0x57:
			printf("0x%x: mov d, a\n", i);
			return i + 1;

		case 0x58:
			printf("0x%x: mov e, b\n", i);
			return i + 1;

		case 0x59:
			printf("0x%x: mov e, c\n", i);
			return i + 1;

		case 0x5a:
			printf("0x%x: mov e, d\n", i);
			return i + 1;

		case 0x5b:
			printf("0x%x: mov e, e\n", i);
			return i + 1;

		case 0x5c:
			printf("0x%x: mov e, h\n", i);
			return i + 1;

		case 0x5d:
			printf("0x%x: mov e, l\n", i);
			return i + 1;

		case 0x5e:
			printf("0x%x: mov e, m\n", i);
			return i + 1;

		case 0x5f:
			printf("0x%x: mov e, a\n", i);
			return i + 1;

		case 0x60:
			printf("0x%x: mov h, b\n", i);
			return i + 1;

		case 0x61:
			printf("0x%x: mov h, c\n", i);
			return i + 1;

		case 0x62:
			printf("0x%x: mov h, d\n", i);
			return i + 1;

		case 0x63:
			printf("0x%x: mov h, e\n", i);
			return i + 1;

		case 0x64:
			printf("0x%x: mov h, h\n", i);
			return i + 1;

		case 0x65:
			printf("0x%x: mov h, l\n", i);
			return i + 1;

		case 0x66:
			printf("0x%x: mov h, m\n", i);
			return i + 1;

		case 0x67:
			printf("0x%x: mov h, a\n", i);
			return i + 1;

		case 0x68:
			printf("0x%x: mov l, b\n", i);
			return i + 1;

		case 0x69:
			printf("0x%x: mov l, c\n", i);
			return i + 1;

		case 0x6a:
			printf("0x%x: mov l, d\n", i);
			return i + 1;

		case 0x6b:
			printf("0x%x: mov l, e\n", i);
			return i + 1;

		case 0x6c:
			printf("0x%x: mov l, h\n", i);
			return i + 1;

		case 0x6d:
			printf("0x%x: mov l, l\n", i);
			return i + 1;

		case 0x6e:
			printf("0x%x: mov l, m\n", i);
			return i + 1;

		case 0x6f:
			printf("0x%x: mov l, a\n", i);
			return i + 1;

		case 0x70:
			printf("0x%x: mov m, b\n", i);
			return i + 1;

		case 0x71:
			printf("0x%x: mov m, c\n", i);
			return i + 1;

		case 0x72:
			printf("0x%x: mov m, d\n", i);
			return i + 1;

		case 0x73:
			printf("0x%x: mov m, e\n", i);
			return i + 1;

		case 0x74:
			printf("0x%x: mov m, h\n", i);
			return i + 1;

		case 0x75:
			printf("0x%x: mov m, l\n", i);
			return i + 1;

		case 0x76:
			printf("0x%x: hlt\n", i);
			return i + 1;

		case 0x77:
			printf("0x%x: mov m, a\n", i);
			return i + 1;

		case 0x78:
			printf("0x%x: mov a, b\n", i);
			return i + 1;

		case 0x79:
			printf("0x%x: mov a, c\n", i);
			return i + 1;

		case 0x7a:
			printf("0x%x: mov a, d\n", i);
			return i + 1;

		case 0x7b:
			printf("0x%x: mov a, e\n", i);
			return i + 1;

		case 0x7c:
			printf("0x%x: mov a, h\n", i);
			return i + 1;

		case 0x7d:
			printf("0x%x: mov a, l\n", i);
			return i + 1;

		case 0x7e:
			printf("0x%x: mov a, m\n", i);
			return i + 1;

		case 0x7f:
			printf("0x%x: mov a, a\n", i);
			return i + 1;

		case 0x80:
			printf("0x%x: add b\n", i);
			return i + 1;

		case 0x81:
			printf("0x%x: add c\n", i);
			return i + 1;

		case 0x82:
			printf("0x%x: add d\n", i);
			return i + 1;

		case 0x83:
			printf("0x%x: add e\n", i);
			return i + 1;

		case 0x84:
			printf("0x%x: add h\n", i);
			return i + 1;

		case 0x85:
			printf("0x%x: add l\n", i);
			return i + 1;

		case 0x86:
			printf("0x%x: add m\n", i);
			return i + 1;

		case 0x87:
			printf("0x%x: add a\n", i);
			return i + 1;

		case 0x88:
			printf("0x%x: adc b\n", i);
			return i + 1;

		case 0x89:
			printf("0x%x: adc c\n", i);
			return i + 1;

		case 0x8a:
			printf("0x%x: adc d\n", i);
			return i + 1;

		case 0x8b:
			printf("0x%x: adc e\n", i);
			return i + 1;

		case 0x8c:
			printf("0x%x: adc h\n", i);
			return i + 1;

		case 0x8d:
			printf("0x%x: adc l\n", i);
			return i + 1;

		case 0x8e:
			printf("0x%x: adc m\n", i);
			return i + 1;

		case 0x8f:
			printf("0x%x: adc a\n", i);
			return i + 1;

		case 0x90:
			printf("0x%x: sub b\n", i);
			return i + 1;

		case 0x91:
			printf("0x%x: sub c\n", i);
			return i + 1;

		case 0x92:
			printf("0x%x: sub d\n", i);
			return i + 1;

		case 0x93:
			printf("0x%x: sub e\n", i);
			return i + 1;

		case 0x94:
			printf("0x%x: sub h\n", i);
			return i + 1;

		case 0x95:
			printf("0x%x: sub l\n", i);
			return i + 1;

		case 0x96:
			printf("0x%x: sub m\n", i);
			return i + 1;

		case 0x97:
			printf("0x%x: sub a\n", i);
			return i + 1;

		case 0x98:
			printf("0x%x: sbb b\n", i);
			return i + 1;

		case 0x99:
			printf("0x%x: sbb c\n", i);
			return i + 1;

		case 0x9a:
			printf("0x%x: sbb d\n", i);
			return i + 1;

		case 0x9b:
			printf("0x%x: sbb e\n", i);
			return i + 1;

		case 0x9c:
			printf("0x%x: sbb h\n", i);
			return i + 1;

		case 0x9d:
			printf("0x%x: sbb l\n", i);
			return i + 1;

		case 0x9e:
			printf("0x%x: sbb m\n", i);
			return i + 1;

		case 0x9f:
			printf("0x%x: sbb a\n", i);
			return i + 1;

		case 0xa0:
			printf("0x%x: ana b\n", i);
			return i + 1;

		case 0xa1:
			printf("0x%x: ana c\n", i);
			return i + 1;

		case 0xa2:
			printf("0x%x: ana d\n", i);
			return i + 1;

		case 0xa3:
			printf("0x%x: ana e\n", i);
			return i + 1;

		case 0xa4:
			printf("0x%x: ana h\n", i);
			return i + 1;

		case 0xa5:
			printf("0x%x: ana l\n", i);
			return i + 1;

		case 0xa6:
			printf("0x%x: ana m\n", i);
			return i + 1;

		case 0xa7:
			printf("0x%x: ana a\n", i);
			return i + 1;

		case 0xa8:
			printf("0x%x: xra b\n", i);
			return i + 1;

		case 0xa9:
			printf("0x%x: xra c\n", i);
			return i + 1;

		case 0xaa:
			printf("0x%x: xra d\n", i);
			return i + 1;

		case 0xab:
			printf("0x%x: xra e\n", i);
			return i + 1;

		case 0xac:
			printf("0x%x: xra h\n", i);
			return i + 1;

		case 0xad:
			printf("0x%x: xra l\n", i);
			return i + 1;

		case 0xae:
			printf("0x%x: xra m\n", i);
			return i + 1;

		case 0xaf:
			printf("0x%x: xra a\n", i);
			return i + 1;

		case 0xb0:
			printf("0x%x: ora b\n", i);
			return i + 1;

		case 0xb1:
			printf("0x%x: ora c\n", i);
			return i + 1;

		case 0xb2:
			printf("0x%x: ora d\n", i);
			return i + 1;

		case 0xb3:
			printf("0x%x: ora e\n", i);
			return i + 1;

		case 0xb4:
			printf("0x%x: ora h\n", i);
			return i + 1;

		case 0xb5:
			printf("0x%x: ora l\n", i);
			return i + 1;

		case 0xb6:
			printf("0x%x: ora m\n", i);
			return i + 1;

		case 0xb7:
			printf("0x%x: ora a\n", i);
			return i + 1;

		case 0xb8:
			printf("0x%x: cmp b\n", i);
			return i + 1;

		case 0xb9:
			printf("0x%x: cmp c\n", i);
			return i + 1;

		case 0xba:
			printf("0x%x: cmp d\n", i);
			return i + 1;

		case 0xbb:
			printf("0x%x: cmp e\n", i);
			return i + 1;

		case 0xbc:
			printf("0x%x: cmp h\n", i);
			return i + 1;

		case 0xbd:
			printf("0x%x: cmp l\n", i);
			return i + 1;

		case 0xbe:
			printf("0x%x: cmp m\n", i);
			return i + 1;

		case 0xbf:
			printf("0x%x: cmp a\n", i);
			return i + 1;

		case 0xc0:
			printf("0x%x: rnz\n", i);
			return i + 1;

		case 0xc1:
			printf("0x%x: pop b\n", i);
			return i + 1;

		case 0xc2:
			printf("0x%x: jnz, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xc3:
			printf("0x%x: jmp, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xc4:
			printf("0x%x: cnz ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xc5:
			printf("0x%x: push b\n", i);
			return i + 1;

		case 0xc6:
			printf("0x%x: adi, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xc7:
			printf("0x%x: rst 0\n", i);
			return i + 1;

		case 0xc8:
			printf("0x%x: rz\n", i);
			return i + 1;

		case 0xc9:
			printf("0x%x: ret\n", i);
			return i + 1;

		case 0xca:
			printf("0x%x: jz, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xcb:
			printf("0x%x: 0xcb instruction not implemented.\n", i);
			return i + 1;

		case 0xcc:
			printf("0x%x: cz, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xcd:
			printf("0x%x: call, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xce:
			printf("0x%x: aci, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xcf:
			printf("0x%x: rst 1\n", i);
			return i + 1;

		case 0xd0:
			printf("0x%x: rnc\n", i);
			return i + 1;

		case 0xd1:
			printf("0x%x: pop d\n", i);
			return i + 1;

		case 0xd2:
			printf("0x%x: jnc, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xd3:
			printf("0x%x: out 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xd4:
			printf("0x%x: cnc ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xd5:
			printf("0x%x: push d\n", i);
			return i + 1;

		case 0xd6:
			printf("0x%x: sui, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xd7:
			printf("0x%x: rst 2\n", i);
			return i + 1;

		case 0xd8:
			printf("0x%x: rc\n", i);
			return i + 1;

		case 0xd9:
			printf("0x%x: 0xd9 instruction not implemented.\n", i);
			return i + 1;

		case 0xda:
			printf("0x%x: jc, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xdb:
			printf("0x%x: in 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xdc:
			printf("0x%x: cc, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xdd:
			printf("0x%x: end 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;
		case 0xde:
			printf("0x%x: sbi, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xdf:
			printf("0x%x: rst 3\n", i);
			return i + 1;

		case 0xe0:
			printf("0x%x: rpo\n", i);
			return i + 1;

		case 0xe1:
			printf("0x%x: pop h\n", i);
			return i + 1;

		case 0xe2:
			printf("0x%x: jpo, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xe3:
			printf("0x%x: xthl\n", i);
			return i + 1;

		case 0xe4:
			printf("0x%x: cpo ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xe5:
			printf("0x%x: push h\n", i);
			return i + 1;

		case 0xe6:
			printf("0x%x: ani, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xe7:
			printf("0x%x: rst 4\n", i);
			return i + 1;

		case 0xe8:
			printf("0x%x: rpe\n", i);
			return i + 1;

		case 0xe9:
			printf("0x%x: pchl\n", i);
			return i + 1;

		case 0xea:
			printf("0x%x: jpe, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xeb:
			printf("0x%x: xchg\n", i);
			return i + 1;

		case 0xec:
			printf("0x%x: cpe, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xed:
			printf("0x%x: 0xed instruction not implemented.\n", i);
			return i + 1;

		case 0xee:
			printf("0x%x: xri, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xef:
			printf("0x%x: rst 5\n", i);
			return i + 1;

		case 0xf0:
			printf("0x%x: rp\n", i);
			return i + 1;

		case 0xf1:
			printf("0x%x: pop psw\n", i);
			return i + 1;

		case 0xf2:
			printf("0x%x: jp, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xf3:
			printf("0x%x: di\n", i);
			return i + 1;

		case 0xf4:
			printf("0x%x: cp ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xf5:
			printf("0x%x: push psw\n", i);
			return i + 1;

		case 0xf6:
			printf("0x%x: ori, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xf7:
			printf("0x%x: rst 6\n", i);
			return i + 1;

		case 0xf8:
			printf("0x%x: rm\n", i);
			return i + 1;

		case 0xf9:
			printf("0x%x: sphl\n", i);
			return i + 1;

		case 0xfa:
			printf("0x%x: jm, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xfb:
			printf("0x%x: ei\n", i);
			return i + 1;

		case 0xfc:
			printf("0x%x: cm, ", i);
			printTwoArgsBack(cpu, i);
			return i + 3;

		case 0xfd:
			printf("0x%x: 0xfd instruction not implemented.\n", i);
			return i + 1;

		case 0xfe:
			printf("0x%x: cpi, 0x%x\n", i, cpu->memory[i + 1]);
			return i + 2;

		case 0xff:
			printf("0x%x: rst 7\n", i);
			return i + 1;
	}
	return i + 1;
}