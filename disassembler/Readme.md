# Python Disassembler

When I was first working on this project, my first step was to write a chart mapping the instructions and what they do to specific opcodes, understanding how the registers/flags for this architecture worked, and writing a disassembler for it (which I did in python).


## Sources

This information you see here is based off of the following resources:

```
https://pmd85.borik.net/instrset/instuction-set.html
http://www.emulator101.com/reference/8080-by-opcode.html
http://pastraiser.com/cpu/i8080/i8080_opcodes.html
```

## Use

to use this disassembler, just provide the binary file you wish to disassemble as an argument:

```
python disassemble.py block4/testcase034
0x0: mvi a, 0x0
0x2: mvi b, 0x25
0x4: mvi c, 0x3
0x7: ldax b
0x8: jnc 0x023
0xc: rlc
0xd: jnz 0x012
0x10: adi 0x1
0x12: cmp b
0x13: jnz 0x018
0x16: mvi a, 0x6
0x18: cmp c
0x19: jnz 0x01e
0x1c: mvi a, 0x23
0x1e: adi 0x1
0x20: jmp 0x06
0x23: nop
0x24: end 0x66
```

## Registers

This architecture has seven `8` bit registers, and three `16` bit registers. It also has 5 flags. In addition to that, the `8080` architecture uses `16` bit addresses:

```
A:	0x8 bit register	Accumulator register
B:	0x8 bit register	Higher 8 bits of BC Register
C:	0x8 bit register	Lower 8 bits of BC Register
D:	0x8 bit register	Higher 8 bits of DE Register
E:	0x8 bit register	Lower 8 bits of DE Register
H:	0x8 bit register	Higher 8 bits of HL Register
L:	0x8 bit register	Lower 8 bits of the HL Register

BC:	0x10 bit register
DE:	0x10 bit register
HL:	0x10 bit register

SP:	0x10 bit Stack Pointer Register 
PC:	0x10 bit Program Counter Register

F:	0x8 bit register	Each bit holds one of the following severn flags

7:	Sign (S Flag)	
6:	Zero (Z Flag)
5:	0 (It always holds the value 0)
4:	Auxillary (AC or H Flag)
3:	0 (It always holds the value 0)	
2:	Parity (P Flag)	
1:	1 (It always holds the value 1)
0:	Carry (C Flag)
```
In addition to that, there are two registers we see `AF` and `m`, which are a bit different:
```
m	-	The register m, isn't a register, it just reference the memory address stored in the HL register.
AF	-	A 0x10 (16) bit register, the higher 8 bits are the A register, and the lower 8 bits are the flags.
```

## Chart

Here is a chrart mapping the individual instructions and their functionallity to specific opcodes

```
0x00	nop			1 byte	Move forward to next instruction
0x01	lxi B, x, y	3 bytes	Moves the second byte (x) into the C register, and the third byte (y) into the B register
0x02	stax b		1 byte	Move contents of A register into the memory address stored in the BC register
0x03	inx b		1 byte	Adds the value one to the BC register
0x04	inr b		1 byte	Adds the value one to the B register
0x05	dcr b		1 byte	Subtracts the value one from the B register
0x06	mov b, x	2 bytes	Moves the value of x into the B register, where X is the second byte
0x07	rlc			1 byte	The contents of the A regisrer are shifted by one to the left. The contents of the seventh bit are moved to the carry flag, and bit 0
0x08	No instruction
0x09	dad b		1 byte	The value of BC is added to the HL register
0x0a	ldax b		1 byte	It dereferences the BC register, and moves that value into the A register
0x0b	dcx b		1 byte	Subtracts the value one from the BC register
0x0c	inr c		1 byte	Adds the value one to the C register
0x0d	dcr c		1 byte	Subtracts the value one from the C register
0x0e	mov c, x	2 bytes	Moves the value of x into the C register, where X is the second byte
0x0f	rrc			1 byte	The contents of the A register are shifted by one to the right. The contents of the 0th bit are moved to the 7th bit, and the carry flag.




0x10	No instruction
0x11	lxi d, x, y	3 bytes	Moves the second byte (x) into the D register, and the third byte (y) into the E register
0x12	stax d		1 byte	Move contents of A register into the memory address stored in the DC register
0x13	inx d		1 byte	Increments the value of the DE register by 1
0x14	inr d		1 byte	Increments the value of the D register by one
0x15	dcr d 		1 byte	Decrements the value of the D register by one
0x16	mov d, x	2 bytes	Moves the value of x into the D register, where X is the second byte
0x17	ral			1 byte	Rotate the contents of the A register by one to the left. The contents of the seventh bit are moved into the Carry flag, and the contents of the carry flag are moved into bit 0.
0x18	No instruction
0x19	dad d		1 byte	Adds the contents of the register D to the HL register
0x1a	ldax d		1 byte	It dereferences the DE register, and moves it's contents into the A register.
0x1b	dcx d		1 byte	This decrements the value of the DE register by one.
0x1c	inr e		1 byte	This increments the value of the E register by one.
0x1d	dcr e		1 byte	This decrements the value of the E register by one.
0x1e	mov e, x	2 bytes	Moves the value of x into the E register, where X is the second byte
0x1f	rar			1 byte	This rotates the contents of the A register by one to the right. The contents of the 0 bit are moved into the carry flag, and the contents of the carry flag are moved into bit 7.




0x20	No instruction
0x21	lxi h, x, y	3 bytes	Moves the second byte (x) into the L register, and the third byte (y) into the H register
0x22	shld addr	3 bytes	Takes the address stored in addr, moves the contents of the L register where it points to, and moves the contents of the H register where addr+1 points to
0x23	inx h		1 byte	Increments the value of the HL register by one
0x24	inr h		1 byte	Increments the value of the H register by one
0x25	dcr h		1 byte	Decrements the value of the H register by one
0x26	mov h, x	2 bytes	Moves the value of x into the h register, where X is the second byte
0x27	daa			1 byte	This is an instruction used after an add instruction, to convert from hex to decimal. The source and destination is the AL register.
0x28	No instruction
0x29	dad h		1 byte	This adds the contents of the H register to the HL register.
0x2a	lhld addr	3 bytes	This takes the two byte (16 bit) address stored in addr, stores the derferenced value in the L register, and the derferenced value of addr+1 into the H register	
0x2b	dcx h		1 byte	This decrements the value of the HL register by one.
0x2c	inr l		1 byte	This increments the value of the L register by one.
0x2d	dcr l		1 byte	This decrements the value of the L register by one.
0x2e	mov l, x	2 bytes	Moves the value of x into the L register, where X is the second byte
0x2f	cma			1 byte	Performs the compliment (bitwise not) on the A register




0x30	No instruction
0x31	lxi sp, x, y	3 bytes	Moves the second byte (x) into the lower 8 bits of the SP register, and the third byte (y) into the higher 8 bits of the SP register
0x32	sta addr	3 bytes	Moves the value of the A register into the area pointed to by the two byte address stored in addr
0x33	inx sp		1 byte	Increments the value of the SP register by one.
0x34	inr m		1 byte	This dereferences the HL register, and increments it's value by one.
0x35	dcr m		1 byte	This dereferences the HL register, and decrements it's value by one.
0x36	mov m, x	2 bytes	Moves the value of x into the address pointed to by the HL register, where X is the second byte
0x37	stc			1 byte	Sets the carry flag equal to one
0x38	No instruction
0x39	dad sp		1 byte	Adds the contents of the SP register to the HL register
0x3a	lda addr	3 bytes	This dereferences the two byte address stored in addr, and loads it's contents into the A register
0x3b	dcx sp		1 byte	This decrements the value of the SP register by one.
0x3c	inr a		1 byte	This increments the value of the A register by one
0x3d	dcr a		1 byte	This decrements the vlaue of the A register by one
0x3e	mov a, x	2 bytes	Moves the value of x into the A register, where X is the second byte
0x3f	cmc			1 byte	Performs the compliment (bitwise not) on the carry flag




0x40	mov b, b	1 byte	Moves the contents of the B register, into the B register
0x41	mov b, c	1 byte	Moves the contents of the C register, into the B register
0x42	mov b, d	1 byte	Moves the contents of the D register, into the B register
0x43	mov b, e	1 byte	Moves the contents of the E register, into the B register
0x44	mov b, h	1 byte	Moves the contents of the H register, into the B register
0x45	mov b, l	1 byte	Moves the contents of the L register, into the B register
0x46	mov b, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the B register
0x47	mov b, a	1 byte	Moves the contents of the A register, into the B register
0x48	mov c, b	1 byte	Moves the contents of the B register, into the C register
0x49	mov c, c	1 byte	Moves the contents of the C register, into the C register
0x4a	mov c, d	1 byte	Moves the contents of the D register, into the C register
0x4b	mov c, e	1 byte	Moves the contents of the E register, into the C register
0x4c	mov c, h	1 byte	Moves the contents of the H register, into the C register
0x4d	mov c, l	1 byte	Moves the contents of the L register, into the C register
0x4e	mov c, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the C register
0x4f	mov c, a	1 byte	Moves the contents of the A register, into the C register




0x50	mov d, b	1 byte	Moves the contents of the B register, into the D register
0x51	mov d, c	1 byte	Moves the contents of the C register, into the D register
0x52	mov d, d	1 byte	Moves the contents of the D register, into the D register
0x53	mov d, e	1 byte	Moves the contents of the E register, into the D register
0x54	mov d, h	1 byte	Moves the contents of the H register, into the D register
0x55	mov d, l	1 byte	Moves the contents of the L register, into the D register
0x56	mov d, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the D register
0x57	mov d, a	1 byte	Moves the contents of the A register, into the D register
0x58	mov e, b	1 byte	Moves the contents of the B register, into the E register
0x59	mov e, c	1 byte	Moves the contents of the C register, into the E register
0x5a	mov e, d	1 byte	Moves the contents of the D register, into the E register
0x5b	mov e, e	1 byte	Moves the contents of the E register, into the E register
0x5c	mov e, h	1 byte	Moves the contents of the H register, into the E register
0x5d	mov e, l	1 byte	Moves the contents of the L register, into the E register
0x5e	mov e, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the E register
0x5f	mov e, a	1 byte	Moves the contents of the A register, into the E register




0x60	mov h, b	1 byte	Moves the contents of the B register, into the H register
0x61	mov h, c	1 byte	Moves the contents of the C register, into the H register
0x62	mov h, d	1 byte	Moves the contents of the D register, into the H register
0x63	mov h, e	1 byte	Moves the contents of the E register, into the H register
0x64	mov h, h	1 byte	Moves the contents of the H register, into the H register
0x65	mov h, l	1 byte	Moves the contents of the L register, into the H register
0x66	mov h, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the H register
0x67	mov h, a	1 byte	Moves the contents of the A register, into the H register
0x68	mov l, b	1 byte	Moves the contents of the B register, into the L register
0x69	mov l, c	1 byte	Moves the contents of the C register, into the L register
0x6a	mov l, d	1 byte	Moves the contents of the D register, into the l register
0x6b	mov l, e	1 byte	Moves the contents of the E register, into the l register
0x6c	mov l, h	1 byte	Moves the contents of the H register, into the l register
0x6d	mov l, l	1 byte	Moves the contents of the L register, into the L register
0x6e	mov l, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the L register
0x6f	mov l, a	1 byte	Moves the contents of the A register, into the L register




0x70	mov m, b	1 byte	Moves the contents of the B register, into the area pointed to by the address stored in the HL register
0x71	mov m, c	1 byte	Moves the contents of the C register, into the area pointed to by the address stored in the HL register
0x72	mov m, d	1 byte	Moves the contents of the D register, into the area pointed to by the address stored in the HL register
0x73	mov m, e	1 byte	Moves the contents of the E register, into the area pointed to by the address stored in the HL register
0x74	mov m, h	1 byte	Moves the contents of the H register, into the area pointed to by the address stored in the HL register
0x75	mov m, l	1 byte	Moves the contents of the L register, into the area pointed to by the address stored in the HL registerr
0x76	hlt			1 byte	Halts the CPU, untill the next external interrupt is fired
0x77	mov m, a	1 byte	Moves the contents of the A register, into the area pointed to by the address stored in the HL register
0x78	mov a, b	1 byte	Moves the contents of the B register, into the A register
0x79	mov a, c	1 byte	Moves the contents of the C register, into the A register
0x7a	mov a, d	1 byte	Moves the contents of the D register, into the A register
0x7b	mov a, e	1 byte	Moves the contents of the E register, into the A register
0x7c	mov a, h	1 byte	Moves the contents of the H register, into the A register
0x7d	mov a, l	1 byte	Moves the contents of the L register, into the A register
0x7e	mov a, m	1 byte	Moves the value pointed to by the address stored in the HL register, into the A register
0x7f	mov a, a	1 byte	Moves the contents of the A register, into the A register




0x80	add b		1 byte	Adds the contents of the A and B registers, and stores it in the A register
0x81	add c		1 byte	Adds the contents of the A and C registers, and stores it in the A register
0x82	add d		1 byte	Adds the contents of the A and D registers, and stores it in the A register
0x83	add e		1 byte	Adds the contents of the A and E registers, and stores it in the A register
0x84	add h		1 byte	Adds the contents of the A and H registers, and stores it in the A register
0x85	add l		1 byte	Adds the contents of the A and L registers, and stores it in the A register
0x86	add m		1 byte	Adds the contents of the A and what the address in the HL register points to, and stores it in the A register
0x87	add a		1 byte	Adds the contents of the A and A registers, and stores it in the A register
0x88	adc b		1 byte	Adds the contents of the A and B registers, and the Carry flag and stores it in the A register
0x89	adc c		1 byte	Adds the contents of the A and C registers, and the Carry flag and stores it in the A register
0x8a	adc d		1 byte	Adds the contents of the A and D registers, and the Carry flag and stores it in the A register
0x8b	adc e		1 byte	Adds the contents of the A and E registers, and the Carry flag and stores it in the A register
0x8c	adc h		1 byte	Adds the contents of the A and H registers, and the Carry flag and stores it in the A register
0x8d	adc l		1 byte	Adds the contents of the A and L registers, and the Carry flag and stores it in the A register
0x8e	adc m		1 byte	Adds the contents of the A and what the address in the HL register points to, and the Carry flag and stores it in the A register
0x8f	adc a		1 byte	Adds the contents of the A and A registers, and the Carry flag and stores it in the A register




0x90	sub b		1 byte	Subtracts the contents of the B register from the contents of the A, and stores it in the A register
0x91	sub c		1 byte	Subtracts the contents of the C register from the contents of the A, and stores it in the A register
0x92	sub d		1 byte	Subtracts the contents of the D register from the contents of the A, and stores it in the A register
0x93	sub e		1 byte	Subtracts the contents of the E register from the contents of the A, and stores it in the A register
0x94	sub h		1 byte	Subtracts the contents of the H register from the contents of the A, and stores it in the A register
0x95	sub l		1 byte	Subtracts the contents of the L register from the contents of the A, and stores it in the A register
0x96	sub m		1 byte	Subtracts the contents of what the address stored in the HL register points to, from the contents of the A, and stores it in the A register
0x97	sub a		1 byte	Subtracts the contents of the A register from the contents of the A, and stores it in the A register
0x98	sbb b		1 byte	Subtracts the contents of the B register and the Carry flag from the contents of the A, and stores it in the A register
0x99	sbb c		1 byte	Subtracts the contents of the C register and the Carry flag from the contents of the A, and stores it in the A register
0x9a	sbb d		1 byte	Subtracts the contents of the D register and the Carry flag from the contents of the A, and stores it in the A register
0x9b	sbb e		1 byte	Subtracts the contents of the E register and the Carry flag from the contents of the A, and stores it in the A register
0x9c	sbb h		1 byte	Subtracts the contents of the H register and the Carry flag from the contents of the A, and stores it in the A register
0x9d	sbb l		1 byte	Subtracts the contents of the L register and the Carry flag from the contents of the A, and stores it in the A register
0x9e	sbb m		1 byte	Subtracts the contents of what the address stored in the HL register points to and the Carry flag, from the contents of the A, and stores it in the A register
0x9f	sbb a		1 byte	Subtracts the contents of the A register and the Carry flag from the contents of the A, and stores it in the A register




0xa0	ana b		1 byte	Ands the contents of the B and A registers, and stores it in the A register
0xa1	ana c		1 byte	Ands the contents of the C and A registers, and stores it in the A register
0xa2	ana d		1 byte	Ands the contents of the D and A registers, and stores it in the A register
0xa3	ana e		1 byte	Ands the contents of the E and A registers, and stores it in the A register
0xa4	ana h		1 byte	Ands the contents of the H and A registers, and stores it in the A register
0xa5	ana l		1 byte	Ands the contents of the L and A registers, and stores it in the A register
0xa6	ana m		1 byte	Ands the contents of the value pointed to by the address stored in  the HL register and the A register, and stores it in the A register
0xa7	ana a		1 byte	Ands the contents of the A and A registers, and stores it in the A register
0xa8	xra b		1 byte	Xors the contents of the B and A registers, and stores it in the A register
0xa9	xra c		1 byte	Xors the contents of the C and A registers, and stores it in the A register
0xaa	xra d		1 byte	Xors the contents of the D and A registers, and stores it in the A register
0xab	xra e		1 byte	Xors the contents of the E and A registers, and stores it in the A register
0xac	xra h		1 byte	Xors the contents of the H and A registers, and stores it in the A register
0xad	xra l		1 byte	Xors the contents of the L and A registers, and stores it in the A register
0xae	xra m		1 byte	Xors  the contents of the value pointed to by the address stored in  the HL register and the A register, and stores it in the A register
0xaf	xra a		1 byte	Xors the contents of the B and A registers, and stores it in the A register



0xb0	ora b		1 byte	Ors the contents of the B and A registers, and stores it in the A register
0xb1	ora c		1 byte	Ors the contents of the C and A registers, and stores it in the A register
0xb2	ora d		1 byte	Ors the contents of the D and A registers, and stores it in the A register
0xb3	ora e		1 byte	Ors the contents of the E and A registers, and stores it in the A register
0xb4	ora h		1 byte	Ors the contents of the H and A registers, and stores it in the A register
0xb5	ora l		1 byte	Ors the contents of the L and A registers, and stores it in the A register
0xb6	ora m		1 byte	Ors the contents of the value pointed to by the address stored in  the HL register and the A register, and stores it in the A register
0xb7	ora a		1 byte	Ors the contents of the A and A registers, and stores it in the A register
0xb8	cmp b		1 byte	Subtracts the contents of the B register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xb9	cmp c		1 byte	Subtracts the contents of the C register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xba	cmp d		1 byte	Subtracts the contents of the D register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xbb	cmp e		1 byte	Subtracts the contents of the E register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xbc	cmp h		1 byte	Subtracts the contents of the H register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xbd	cmp l		1 byte	Subtracts the contents of the L register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0
0xbe	cmp m		1 byte	Subtracts the value pointed to by the address stored in the HL register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0 HL register and the A register, and stores it in the A register
0xbf	cmp a		1 byte	Subtracts the contents of the A register from the A register, if it is equal to zero the Zero flag is set equal to 1, if not then the Zero flag is set to 0




0xc0	rnz			1 byte	If the Zero flag is equal to 0, return
0xc1	pop b		1 byte	The memory location pointed to by the address stored in the SP register, is moved into the C register. Then the SP register is incremented, and then the value pointed to by it's address is moved into the B register, and the SP register is incremented again
0xc2	jnz adr		3 bytes	If the Zero flag is equal to 0 jump to the address where adr is (last two bytes of instruction)
0xc3	jmp adr		3 bytes	Jump to the address stored in adr
0xc4	cnz adr		3 bytes	If the Zero flag is equal to 0, push the current PC plus 3 onto the stack, and call the address where adr is the last two bytes of the instruction
0xc5	push b		1 byte	The SP register is decremented, and the contents of B is stored where the address of SP points to. The SP register is decremented again, and the contents of C is stored where the address os SP points to
0xc6	adi x		2 bytes	Adds the value of x (the second byte of the instruction) to the A register
0xc7	rst 0		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0000 moved into it
0xc8	rz			1 byte	If the value of the Zero flag is equal to one, pop the top stack value into the PC register
0xc9	ret			1 byte	The top stack value is popped into the PC register
0xca	jz adr		3 bytes	If the zero flag is equal to one, the value adr (last two bytes of the instruction) are moved into the PC register
0xcb	No instruction
0xcc	cz adr		3 bytes	If the zero flag is equal to 1, the current value of the PC register plus three is pushed onto the stack, then adr (last two bytes of the instruction) is moved into the PC register
0xcd	call adr		3 bytes	The current value of the PC register plus three is pushed onto the stack, then adr (last two bytes of the instruction) is moved into the PC register
0xce	aci x		2 bytes	The value of the Carry flag and x (second byte of the instruction) are added to the A register
0xcf	rst 1		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0008 moved into it




0xd0	rnc			1 byte	If the Carry flag is equal to 0, return
0xd1	pop d		1 byte	The memory location pointed to by the address stored in the SP register, is moved into the E register. Then the SP register is incremented, and then the value pointed to by it's address is moved into the D register, and the SP register is incremented again
0xd2	jnc adr		3 bytes	If the Carry flag is equal to 0 jump to the address where adr is (last two bytes of instruction)
0xd3	out x		2 bytes	The value of A is written to the port x (x is the second byte of the instruction)
0xd4	cnc adr		3 bytes	If the Carry flag is equal to 0, push the current PC plus 3 onto the stack, and call the address where adr is the last two bytes of the instruction
0xd5	push d		1 byte	The SP register is decremented, and the contents of D is stored where the address of SP points to. The SP register is decremented again, and the contents of E is stored where the address os SP points to
0xd6	sui x		2 bytes	Subtracts the value of x (the second byte of the instruction) from the A register
0xd7	rst 2		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0010 moved into it
0xd8	rc			1 byte	If the value of the Carry flag is equal to one, pop the top stack value into the PC register
0xd9	No instruction
0xda	jc adr		3 bytes	If the Carry flag is equal to one, the value adr (last two bytes of the instruction) are moved into the PC register
0xdb	in x		2 bytes	A byte from port x is written to the A register
0xdc	cc adr		3 bytes	If the Carry flag is equal to 1, the current value of the PC register plus three is pushed onto the stack, then adr (last two bytes of the instruction) is moved into the PC register
0xdd	No instruction
0xde	sbi x		2 bytes	The value of the Carry flag and x (second byte of the instruction) are subtracted from the A register
0xdf	rst 3		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0018 moved into it




0xe0	rpo			1 byte	If the Parity flag is equal to 0, return
0xe1	pop h		1 byte	The memory location pointed to by the address stored in the SP register, is moved into the L register. Then the SP register is incremented, and then the value pointed to by it's address is moved into the H register, and the SP register is incremented again
0xe2	jpo adr		3 bytes	If the Parity flag is equal to 0 jump to the address where adr is (last two bytes of instruction)
0xe3	xthl		1 byte	Moves the pointer stored in the L location into SP, and the pointer stored in H into SP+1
0xe4	cpo adr		3 bytes	If the Parity flag is equal to 0, push the current PC plus 3 onto the stack, and call the address where adr is the last two bytes of the instruction
0xe5	push h		1 byte	The SP register is decremented, and the contents of H is stored where the address of SP points to. The SP register is decremented again, and the contents of L is stored where the address os SP points to
0xe6	ani x		2 bytes	Performs a bitwise and on the A register, and x (second byte of the instruction)
0xe7	rst 4		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0020 moved into it
0xe8	rpe			1 byte	If the value of the Parity flag is equal to one, pop the top stack value into the PC register
0xe9	pchl		1 byte	Loads the contents of the HL register into the PC register
0xea	jpe adr		3 bytes	If the Parity flag is equal to one, the value adr (last two bytes of the instruction) are moved into the PC register
0xeb	xchg		1 byte	Exchanges the contents of the DE and HL registers
0xec	cpe adr		3 bytes	If the Parity flag is equal to 1, the current value of the PC register plus three is pushed onto the stack, then adr (last two bytes of the instruction) is moved into the PC register
0xed	No instruction
0xee	xri x		2 bytes	The value of the x (second byte of the instruction) is xored with the A register
0xef	rst 5		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0028 moved into it




0xf0	rp			1 byte	If the Sign flag is equal to 0, return
0xf1	pop psw		1 byte	The memory location pointed to by the address stored in the SP register, is moved into the F register. Then the SP register is incremented, and then the value pointed to by it's address is moved into the A register, and the SP register is incremented again
0xf2	jp adr		3 bytes	If the Sign flag is equal to 0 jump to the address where adr is (last two bytes of instruction)
0xf3	di			1 byte	This resets the interrupt flip-flops, which prevents interrupt from triggering
0xf4	cp adr		3 bytes	If the Sign flag is equal to 0, push the current PC plus 3 onto the stack, and call the address where adr is the last two bytes of the instruction
0xf5	push psw	1 byte	The SP register is decremented, and the contents of A is stored where the address of SP points to. The SP register is decremented again, and the contents of F is stored where the address os SP points to
0xf6	ori x		2 bytes	Performs a bitwise or on the A register, and x (second byte of the instruction)
0xf7	rst 6		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0030 moved into it
0xf8	rm			1 byte	If the value of the Sign flag is equal to one, pop the top stack value into the PC register
0xf9	sphl		1 byte	Loads the contents of the HL register into the SP register
0xfa	jm adr		3 bytes	If the Sign flag is equal to one, the value adr (last two bytes of the instruction) are moved into the PC register
0xfb	ei		1 byte	This sets the interrupt flip-flops, which allows for interrupts
0xfc	cm adr		3 bytes	If the Parity flag is equal to 1, the current value of the PC register plus three is pushed onto the stack, then adr (last two bytes of the instruction) is moved into the PC register
0xfd	No instruction
0xfe	cpi x		2 bytes	The value of the x (second byte of the instruction) is subtracted from the A register, and sets the corresponding flags based off of it. The value of the A register isn't changed.
0xff	rst 7		1 byte	Pushes the value of the PC register plus one onto the stack, then the PC register has the value 0x0038 moved into it

```
