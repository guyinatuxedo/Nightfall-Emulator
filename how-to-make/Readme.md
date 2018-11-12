# How to Make

This is a little writeup on how I designed the emulator, disassembler, and debugger. I won't get into much detail, since the source code is in this repo. Also in this directory I threw in a copy of the 8080 instruction manual that I used to make it, which is from `http://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf`.

## Disassembler

For this, first I read the contents of the binary file, then fed it through a switch statement. It would map the op codes to the respective instructions. Since all of the opcodes are only one byte, for each loop of the switch statement I would increment the index which the switch statement reads by one, so it would move on to the next opcode for the next loop. For some of the opcodes that take 1 argument, I would print the agrument along with the instruction then increment the index by 2, so it moves on to the next instruction and doesn't try to map the argument to an instruction. For opcodes with two arguments, I would print the two arguments with the instruction, and increment the index by 3 for the same reason.

## Emulator

#### Registers / Memory Mapping

When I designed this portion, I made a struct which represents the portions of the 8080 cpu / ram that we would need to keep track of for this emulator:

```
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

	// I excluded the rest of the struct, since it deals with the debugger aspect
```

Since the registers `a`, `b`, `c`, `d`, `e`, `h`, and `l` hold 8 bits, I just established a `uint8_t` which is an unsigned integer which can only hold 8 bits. When I would need to reference one of the 16 bit registers `bc`, `de`, or `hl` I would just have a function that would convert the `uint8_t` values into the corresponding 16 bit value. For the flags, I decided to just make 8 bools. each representing one of the flags, since a lot of our use for them deals with individual bits. For the 16 bit registers `sp` and `pc`, I decided to just make them 16 bit `uint16_t`, since we usually want the whole 16 bits when we deal with them. 

For the memory of the emulator, since we are dealing with a 16 bit architecture, we have 16 bit addresses. As a result all possible memory must map to somewhere between `0x0000 - 0xffff`. Since this is about a half of a megabyte, I decided to just allocate it all with malloc, to get one massive array that contains all of the memory. Then I would treat addresses as indexes for the array. I loaded the code which is run starting at `0x0`, and I started the stack at `0x4000` (rememeber the stack grows downwards, towards `0x0`). I don't recall ever seeing what the standard was for where to start the stack / code regions, so I just picked those values for no concrete reason other than convinience.

Also for the `m` register, since that is just the memory address pointed to by the `hl` register, I would just convert the `hl` register contents into an address, then treat that as a register. 

Also another thing to keep in mind, the stack pointer is stored in the `sp` register, and the program counter is stored in the `pc` register (points the current instruction being ran).

#### Flags

Setting flags is something that we would have to do for a lot of different functions. So I just wrote some functions to handle it for me, and called them whenever I need to set the flags. Also if you don't know what binary anding, oring, and shifting is, you will probably want to look that up.

##### Zero

Since this flag I just checked to see if the result is equal to zero, and if it is, I would set the zero fla to true.

##### Sign

The sign bit is the last bit (the seventh bit, remember it is 0 indexed), and this flag essentially just checks to see if  that bit is set. So to set this flag, we can just see if that bit is set, by anding it with `0x80` since in binary it is `0b10000000`, if it isn't zero, that means that the sign bit is set. After that we would set the flag accordingly.

##### Auxiliary

This flag checks if there is a carry out of bit 3. A carry out occurs when the operation of one bit affects the next bit, so with addition it is when both values are `1`, and with subtraction it is when you subtract a `1` from a `0`. To see if this bit is set for a value, we can just and it with `0x8` since that maps to `0b1000` which is the third bit. For this function I also passed it a value to indicate whether it is a subtraction, or addition operation, and I would also pass it the two values instead of the final result. For addition if both of the third bits were set, I would set the auxillary flag. For subtraction, if the value being subtracted had the third bit set, and the value being subtracted from didn't I would set the auxillary flag.

##### Parity

This flag is a bit different from the rest. If it is set it indicates that the result of whatever operation happened has an even number of `1`s (for the binary string). If it isn't set then the number of `1`s is odd. So for the value `0x73` it's binary string is `0b1110011`, which has 5 `1`s, and since that is odd, the Parity flag would not be set. So to figure this out, I just ran the result through a for loop that would and the result with `0x1` (which in binary is just 0b1), increment an integer by one if the result isn't one (meaning that that bit is set), shift it over by 1, and repeat seven times. Then the count integer would tell us how many bits are set. Proceeding that we could just mod the count by `2`, and if the result wasn't `0` we knew that the amount of `1`s in the result was odd (and vice versa).

##### Carry

Like the Auxiliary flag, whether this is a subtraction or addition operation changes if the flag is set (but we only need to look at the result). This flag looks at if the eight bit is set or not (which maps to `0x100` or `0b100000000`). This can only happen if there is a carry out of the seventh bit, since we are dealing wiht 8 bit values. This flag is extreamly useful since if we add two values together such as `0xac` and `0x66`, although it would yeild `0x112` it would only store `0x12` since we are dealing with 8 bit values. This allows us to determine if the 8 bits captures the entire picture. For addition, we just check if the 8th bit is set, and if it is we set the Carry flag. However for subtraction it works the opposite way, if the 8th bit is set, the Carry flag isn't set.

Also for 16 bit values, we use the same logic, however we check if the 16th bit (which maps to `0x10000` or `0b10000000000000000`) is set.

#### Instructions

Here is what I did for all of the instructions. Also for instructions that took arguments, I would increment the index by the length of the argument, to account for that (just like with the disassembler).

Also one thing I would like to say, how the manual describes subtraction is with the use of twos complement logic. How that works is it takes the value that will be subtracted from something, inverts it, then adds one to it. It then takes this new value and adds it to whatever it is being added to. I have implemented this logic at some places throughout the code for my emulator.

##### Nop

For this I just had it do nothing, since that's what a NOP essentially does.

##### lxi

For these instructions, I would just load the next two bytes into the corresponding 16 bit register.

##### stax

For this, I would treat the contents of either the `bc` or `de` register as an address, then store the contents of the `a` register in it.

##### shld & sta

For this, I would treat the next two bytes (which are args) as an address, then store the contents of either the `a` or `hl` register (depends on the instruction used) in there.

##### inx & dcx

These just increment or decrement a 16 bit register by one. For the 16 bit register pairs `bc`, `de`, and `hl`, for incrementing I would check to see if the lower eight bits is lower than `0xff`. If it is, I would just increment the lower 8 bit register by one. If the lower 8 bits is `0xff`, then I would set the lower 8 bits to `0x0`, then increment the higher 8 bits. Also if the 16 bit register pair was `0xffff`, I would follow the standard and set it the pair to `0x0000`. For subtracting from a 16 bit register pair, I used the same sort of logic. If the last 8 bits was greater than `0x0`, I would just decrement it by one. If it wasn't I would set it equal to `0xff` and decrement the higher 8 bits by one. And if the 16 bit register pair is `0x0000` I would have it wrap around to `0xffff`.

When dealing with the `sp` register, since it isn't a register pair it's a bit simpler. I check to see that there isn't a wrap around, and if there isn't I just either increment or decrement the regitser.

##### inr & dcr

This just increments / decrements the 8 bit register passed to it. It also takes the result and runs it through the flag set functions for the flags zero, sign, parity, auxiliary, and carry.

##### mvi

For this, I would just move the one byte argument into the corresponding register.

##### rlc & rrc

For these instructions, I would rotate the contents of the `a` register to either the right or the left. For the bit that would loop around (when shifting to the left it would be the leftmost bit, and vice versa) it would also be copied to the carry bit.

##### ral & rar

Pretty similar to the rlc & rrc, except the bit that would of been looped around gets moved to the carry bit, and whatever was in the carry bit gets looped around.

##### daa

For this, I just read the manual, and implemented it the best way I thought of.

##### stc

This just sets the carry flag, so the carry flag is equal to true.

##### cma & cmc

These instructions just invert the `a` register, and carry flags, so that is what I did.

##### dad

this just moves the contents of the `16` bit register into hl, so that's what I did. For the register pairs, it was pretty straight forward. For mocing the sp register, there was a bit of shifting involved due to the difference in  data types.

##### ldax, lhld, lda

For all of these instructions, they all deal with with dereferencing an address and loading that value somewhere. For ldax, I would conver the register pair into a 16 bit value, then treat that as an address. Lda is pretty similar to ldax, except the address is from an argument. Also with lhld we load a 16 bit value instead of an 8 bit value.

##### mov

All of the mov instructions are pretty straight forward. We just set one register equal to another. For the m register, we just treat the contents of the hl register as an address, dereference it, then treat that as a register,

##### hlt

For this instruction, since I didn't implement hardware interrupts I essentially just had it be a NOP.

##### add & sub

For these instructions, I would just subtract the contents of the eight bit register from the a register, store the result as a 16 bit `uint16_t` to store because of the carry flag, run it through the set flag functions for Carry, Sign, Zero, Parity, and Auxiliary, then store the lower 8 bits of the result in a.

##### adc & sbb

This is pretty similar to add & sub. The only difference is the number being added to, or subtracted from A has the int conversion of the carry flag added to it (`1` if it is set, `0` if not).

##### ana 

For this, I just and whatever 8 bit register is being used with the A register, store the result in A, and run the result through the Zero, Sign, Parity. Since anding does not lead to a binary carry out, the Carry flag will always be 0. According to the manual, the auxiliary bit isn't affected, otherwise like the Carry flag I would set it to 0.

##### xra & ora

Like the ana instruction, it performs it's respective binary operation (either xor or or) on whatever register is being used against the A register, and stores the result in the A register. Unlike the ana instruction, the Auxiliary flag is affected and since a binary carryout cannot happen with xor or or, both of those flags are set equal to zero. The result is also sent through the flag set functions for Zero, Sign, and Parity.

##### cmp

This instruction compares a value against the A register, by simply subtracting it from A, but the value of A is not changed. So what I did is I just had a value be equal to `A - <register>`, then ran that value through the set flag functions for Carry, Sign, Zero, Parity, and Auxiliary. 

##### push

This instruction moves the contents of a 16 bit register to the stack. It does so by decrementing the stack pointer by 1 (since each value is just 8 bits, which is 1 byte, and the stack grows downards), sets the next value equal to the higher 8 bits of the register, decrements it again, then sets the new value equal to the lower 8 bits. The contents of the 16 bit register are not changed.

##### pop

This instruction is like push in reverse, and the 16 bit register's value is affected. It sets the lower eight bits of the register equal to whatever the current stack pointer points to, increments by one, sets the higher eight bits equal to whatever the new stack pointer points to, then increments it again by one. 

##### jmp

For this, I would just take the two byte address as the argument, shift it around into a 16 bit address assuming that the arguments were in least endian (least signifcant byte first), then set the program counter (which controls which instruction is being executed) equal to that address.

##### call

This is pretty similar to the jmp instrunction. The only difference is the address of the next instruction (which at the time it is called is pc + 3) onto the stack, which will act as a return address. That way when the branch ends, it will be able to continue execution right after the call.

##### ret

This instruction happens at the end of a branch. Essentially it just pops the 16 bits pointed to by the stack pointer (which should be the return address) into the `pc` register. That way the branch finishes and the execution continues on from the call instruction.

##### rest of ret, call, and jmp instructions

there are about 24 other instructions for jmp, call, and ret. All of these due is just check the condition of one of the flags, and based upon it's value it may or may not run the instruction. I just implement the same logic for the instructions. 

##### out & in

Since I didn't implement other hardware interfaces, I just had these be NOPs essentially.

##### xthl

For this, I dereferenced the addresses stored in sp (and the address of sp + 1), and swapped it's contents with that of the H and L registers.

##### xchg

For this I just swa[[ed the contents of the DE and HL registers.

##### di & ei

Even though I did not implement hardware interrupts, with the cup8080 struct, I included a bool value which storred whether or not hardware interrupts were allowed. These instructions would just assing that bit it's corresponding value.

##### spchl & pchl

Using a bit of binary logic, I would just move the contents of the HL register into the SP and PC registers.

##### adi, aci, sui, and sbi

Basically the same as the add, adc, sub, and sbb instructions, except instead of dealing with a separate register, we deal with an argument to the instruction.

##### ani, xri, ori, cpi

Basically the same as the ana, xra, ora, and cmp instructions, except instead of dealing with a separate register, we deal with an argument to the instruction.

##### rst

For this I just push the current program counter plus one onto the stack to act as a return address, then load the corresponding address (based upon which rst instruction it is) into the `pc` register.

## Debugger

Here I'm just going to go through breifly how implemented the various debugging functionallity for my debugger. Also here is the portion of the `cpu8080` struct which deals with the debugging functionalities.

```
	// a ptr to the breakpoints
	breakpoints *breakpoints;
	
	// a bool to indicate if it is running
	bool running;

	bool breaked;

	// Here are some values, which will keep track of where the various regions of memory are 
	uint16_t memBeg;	// Beginning of entire memory region
	uint16_t memEnd;	// End of entrie memory region
	uint16_t codeBeg;	// Beginning of where code is loaded
	uint16_t codeEnd;	// End of where code is loaded
	uint16_t stackBeg;	// Beginning of the stack
	// the end of the code is kept by codeEnd, and the end of the stack is kep by sp

	bool disasAsExec; // A bool to keep track of whether or not to disassmble intructions as they are executed

	uint8_t step; // A value to keep track of the staus of steeping through instructions, for the step instruction command

	// a bit which controls whether interrupts can happen, currently don't use it for anything. 1 for allow, 0 for disable.
	bool allowInterrupt;

} cpu8080;
```
##### Help

This is essentially just a puts statement.

##### Disassembly

For this, I just run a for loop feeding the code through the disassemble function, untill I reach the codeend, which is determined when the code is loaded. Also another thing to note, the disassemble function returns the index of the next instruction.

##### toggle disassembly

Every time a new instruction is ran, I check to see if the toggle disassembly bool is true. If it is, I run the instruction being ran through the disasAsExec function.

##### dump memory

I just open up the file `memory.dat` (create it if it's not made) and just write the contents of all of the memory to it. 

##### display register / registers

This is for the `i` and `p` commands. Depending on the command and the argument, I just print the contetns of the registers.

##### run

Starting off I run it through the `resetExeuction` function to essentially clear out the state of the emulation so we start it from scratch, then run it through the `emulate` function which actually handles the emulation.

##### continue 

Basically the run function, howoever we don't run it through the `resetExecution` function, so it continues from where you left off versus starting from scratch.

##### quit 

For this, I essentially free up the dynamically allocated memory using `destroyCpu` function, and freeing `menuChoice`, then returning from the `menu` function (which sould close the emulator). 

##### breakpoints

For breakpoints, I have them be a linked list (I could probably get a better runtime with a different data structure, but due to time constraints I didn't do that) which I maintain a head ptr. The struct for the breakpoints is this:

```
typedef struct breakpoints {
	uint16_t breakp;
	bool active;
	struct breakpoints *next;
} breakpoints;
```

Essentially I just keep track of whether or not the breakpoint is active, what instruction the breakpoint is, and what the next breakpoint address is. Also I keep the head pointer for the breakpoints in the cpu in the ptr `breakpoints`. Each time I want to check for a breakpoint, I just loop through the list untill I either get to the end, or find a breakpoint in the linked list with the instruction (hence the thing about their being a data structure with a better run time). For adding / editing breakpoints, it's pretty much standard linked list operations.

Also when a breakpoint is reached, I just exit the emulate function, which brings us back to the debugger menu.

##### jump

For this, I just convert the argument into an integer, and then set the `pc` register equal to it.

##### view memory mappings

When a cpu8080 struct is created, I store pointers to the various memory regions in the cpu struct (things like `stackBeg` and `codeEnd`), which then I just print those addresses.

##### step

For this, I kept a value `step` in the cpu8080 struct. Essentially there is a three stage process to this. First it sets `step` equal to 1, then runs it through the emulate function. Then the `step` value would get switched to `2` indicating that an instruction has been run, then switched back to `0`, and it would go back to the the debug menu. The checking occurs towards the start of each iteration of the for loop in the `emulate` function.

##### examine

For this, we take the address that we are given, dereferences it, then prints the value as an 8 bit byte. If the argument <amnt> is given, then we will print `<amnt>` bytes from the address given, in an ascending order.

##### set

If the argument that is given is a register, we then set the corresponding to register to the given value. If an address is given, then I dereference the address and set the corresponding value to the given argument.
