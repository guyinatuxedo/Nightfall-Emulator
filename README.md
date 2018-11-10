# Nightfall 
## 8080 Emulator / Debugger / Disassembler

This is a custom emulator that I wrote for the 16 bit intel 8080 architecture, that is also a debugger, and a disassembler. I mainly wrote this, so I could learn how to make these things.

## Compiling

To compile the code, you can just use make. All of the source code is under `src`:

```
$	cd src/
$	make
```

## Disassembler

When I was first working on this, I made a python disassembler, which you can find under `disassembler`.

## How to make

I documented how to make an emulator, debugger, and disassembler under `how-to-make`.

## Use

To use the emulator, it takes a single argument which is the binary file you wish to emulate:

```
$	./Nightfall block2/testcase007
Nightfall 8080 Emualtor / Debugger / Disassembler
ndb> 
```

you can disassemble a loaded file with the `e` command:
```
ndb> e
0x0: mvi b, 0x4
0x2: mvi c, 0x10
0x4: mvi d, 0x88
0x6: mvi e, 0xba
0x8: mvi h, 0x5
0xa: mvi l, 0x6
0xc: mvi m, 0x27
0xe: mvi a, 0x4
0x10: add b
0x11: add c
0x12: add d
0x13: add e
0x14: add h
0x15: add l
0x16: add m
0x17: add a
0x18: end 0x66
```

to emulate the binary, use `r`:
```
ndb> r
```

to set a breakpoint for `0x6`, use `b <adr>`:
```
ndb> b 0x6
ndb> r
Breakpoint hit at 0x6
```

to list all breakpoints, use `z`:
```
ndb> z
Breakpoint 0x0 at 6		(active: 1)
Breakpoint 0x1 at a		(active: 1)
Breakpoint 0x2 at e		(active: 1)
```

to deactivate a breakpoint, use `d <breakpoint>`:
```
ndb> d 0x2
ndb> z
Breakpoint 0x0 at 6		(active: 1)
Breakpoint 0x1 at a		(active: 1)
Breakpoint 0x2 at e		(active: 0)
```

to activate a breakpoint, use `a <breakpoint>`:
```
ndb> a 0x2
ndb> z
Breakpoint 0x0 at 6		(active: 1)
Breakpoint 0x1 at a		(active: 1)
Breakpoint 0x2 at e		(active: 1)
```

to continue execution after you have hit a breakpoint, use `c`:

```
ndb> r
Breakpoint hit at 0x6
ndb> c
Breakpoint hit at 0xa
ndb> c
Breakpoint hit at 0xe
ndb> c
```

to list the contents of all of the registers and flags, use `i`:
```
ndb> r
Breakpoint hit at 0x6
ndb> i
8 bit registers
register a: 0x0
register b: 0x4
register c: 0x10
register d: 0x88
register e: 0x0
register h: 0x0
register l: 0x0

16 bit registers
register bc: 0x410
register de: 0x8800
register hl: 0x0
register af: 0x2

Flags
7: Sign flag: 0
6: Zero flag: 0
5: zero1 flag (value shouldn't change): 0
4: Auxillary flag: 0
3: zero2 flag (value shouldn't change): 0
2: Parity flag: 0
1: one flag (value shouldn't change): 1
0: Carry flag: 0

Stack registers (16 bit)
register pc: 0x6
register sp: 0x4000

Interrupt bit: 0x1
```

to jump to a different place of the code manually, use `j` (you will need to continue using `c` afterwards):
```
ndb> z
Breakpoint 0x0 at 6		(active: 1)
Breakpoint 0x1 at a		(active: 1)
Breakpoint 0x2 at e		(active: 1)
ndb> r
Breakpoint hit at 0x6
ndb> j 0xe
ndb> c
Breakpoint hit at 0xe
```

to list the memory mapping use `m`(just tells you where the stack, code, and total memroy regions are):

```
ndb> r
Breakpoint hit at 0x6
ndb> m
Total Memory Region:	 0x0 - 0xffff
Code Memory Region:	 0x0 - 0x19
Stack Memory Region:	 0x4000 - 0x4000
```

to print the contents of a register, use `p <register>`:
```
ndb> r
Breakpoint hit at 0x6
ndb> p d
register d: 0x88
ndb> p de
register de: 0x8800
```

to quit the debugger, use `q`:
```
ndb> r
Breakpoint hit at 0x6
ndb> q
```

to step an instruction, use `s` (just execute the next instruction):
```
ndb> r
Breakpoint hit at 0x6
ndb> p e
register e: 0x0
ndb> s
Stepped to 0x8
ndb> p e
register e: 0xba
```

to toggle disassemble use `t` (disassembles all instructions that are ran):
```
ndb> t
ndb> r
0x0: mvi b, 0x4
0x2: mvi c, 0x10
0x4: mvi d, 0x88
Breakpoint hit at 0x6
ndb> c
0x6: mvi e, 0xba
0x8: mvi h, 0x5
Breakpoint hit at 0xa
ndb> c
0xa: mvi l, 0x6
0xc: mvi m, 0x27
Breakpoint hit at 0xe
ndb> c
0xe: mvi a, 0x4
0x10: add b
0x11: add c
0x12: add d
0x13: add e
0x14: add h
0x15: add l
0x16: add m
0x17: add a
0x18: end 0x66
```

to dump the contents of the memory to the file `memory.dat`, use `u` (dumps all memory between `0x0` - `0xffff`):
```
ndb> r
Breakpoint hit at 0x6
ndb> u
ndb> q
$	ls | grep memory.dat
memory.dat
```

to examine a memory address, use `x <addr>` (this example uses a different testcase):
```
./Nightfall block1/testcase004
Nightfall 8080 Emualtor / Debugger / Disassembler
ndb> e
0x0: mvi h, 0x20
0x2: mvi l, 0x40
0x4: mov c, l
0x5: mov a, l
0x6: mov d, h
0x7: mov b, a
0x8: mvi c, 0x0
0xa: mov c, b
0xb: mov e, b
0xc: mov e, d
0xd: mov m, b
0xe: mov m, c
0xf: mov m, d
0x10: mov m, e
0x11: mov m, h
0x12: mov m, l
0x13: mov m, a
0x14: end 0x66
ndb> b 0xd
ndb> r
Breakpoint hit at 0xd
ndb> p b
register b: 0x40
ndb> p hl
register hl: 0x2040
ndb> x 0x2040
0x2040: 0x0
ndb> s
Stepped to 0xe
ndb> x 0x2040
0x2040: 0x40
```

to set a register, or memory address to a specific `8` bit value, use either `y <register> <value>` or `y <addr> <value>`. This uses testcase004 (same as `x`) :
```
ndb> r
Breakpoint hit at 0xd
ndb> p hl
register hl: 0x2040
ndb> x 0x2040
0x2040: 0x0
ndb> p b
register b: 0x40
ndb> y b 0x66
ndb> p b
register b: 0x66
ndb> s
Stepped to 0xe
ndb> x 0x2040
0x2040: 0x66
```
or to set an address equal to a value:

```
ndb> x 0x2040
0x2040: 0x40
ndb> y 0x2040 0x66
ndb> x 0x2040
0x2040: 0x66
```

to print the help menu:
```
ndb> h
Help Menu
a <breakpoint id>	-	Activate a previously deacticated breakpoint
b <code address>	-	Set a breakpoint at the address
c					-	Continue execution from a breakpoint
d <breakpoint id>	-	Deactivate a breakpoint
e					-	Disassemble all code in binary
h					-	Print help menu
i					-	Display values of all registers and flags
j <next adr>		-	Set the next address to be executed (essentially a jump, but need to continue)
m					-	Print the addresses for memory regions
p <register>		-	Prints contents of register
q					-	Quit the emulator/debugger
r					-	Run the emulator from the start
s					-	Step to the next instruction
t					-	Toggle running disassembly, which disassembles instructions as they are ran
u					-	Dump the contents of memory to the file memory.dat
x <amnt> <adr>		-	Print amnt 8 bit segments starting at adr (if single argument given, amnt is 1)
y <target <value>	-	Set target register or address to value
z 					-	Show all breakpoints, and if they are activated
```

## Sources

Here are some sources which have really helped me with writing this:
```
https://pmd85.borik.net/instrset/instuction-set.html
http://www.emulator101.com/reference/8080-by-opcode.html
http://pastraiser.com/cpu/i8080/i8080_opcodes.html
http://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf
^8080 manual
http://www.emulator101.com/
```
