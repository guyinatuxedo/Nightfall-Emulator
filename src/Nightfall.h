#include <stdio.h>

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

	// The rest of these are variables for debugging purposes

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

// Here are some constants I declared, for things such as size of memory region,
// where the stack starts, and the file which memory is dumped to

#define menuInputSize 20
#define memorySize 0xffff
#define startStack 0x4000
#define memoryOutputFile "memory.dat"

// functions for emulation and disassembling

void emulate(cpu8080 *cpu);

int disass(cpu8080 *cpu, int i);

// Here are the functions which deal with setting flags

void auxiliaryFlag(cpu8080 *cpu, uint8_t x, uint8_t y, uint8_t z);

void zeroFlag(cpu8080 *cpu, uint16_t x);

void signFlag(cpu8080 *cpu, uint16_t x);

void parityFlag(cpu8080 *cpu, uint16_t x);

void carryFlag(cpu8080 *cpu, uint16_t x);

void carryFlagSub(cpu8080 *cpu, uint16_t x);

void carryFlag16(cpu8080 *cpu, uint32_t x);

// Here are the functions which deal with math functions, incrementing and decrementing 

void add(cpu8080 *cpu, uint8_t x);

void adc(cpu8080 *cpu, uint8_t x);

void sub(cpu8080 *cpu, uint8_t x);

void sbb(cpu8080 *cpu, uint8_t x);

void and(cpu8080 *cpu, uint8_t x);

void xor(cpu8080 *cpu, uint8_t x);

void or(cpu8080 *cpu, uint8_t x);

void inr(cpu8080 *cpu, uint8_t *reg);

void dcr(cpu8080 *cpu, uint8_t *reg);

void inx8(uint8_t *x, uint8_t *y);

void inx16(uint16_t *x);

void dcx8(uint8_t *x, uint8_t *y);

void dcx16(uint16_t *x);

// These are functions which essentially deal with moving values around

void stax(cpu8080 *cpu, uint8_t x, uint8_t y);

void ldax(cpu8080 *cpu, uint8_t x, uint8_t y);

void mvi(cpu8080 *cpu, uint8_t *reg, uint8_t arg);

void lxi(cpu8080 *cpu, uint8_t *x, uint8_t *y);

void lxi16(cpu8080 *cpu, uint16_t *x);

void dad(cpu8080 *cpu, uint8_t *x, uint8_t *y);

void dad16(cpu8080 *cpu, uint16_t *z);

// Here are the functions which deal with poping / pushing

void pop(cpu8080 *cpu, uint8_t *x, uint8_t *y);

void push(cpu8080 *cpu, uint8_t x, uint8_t y);

void push16(cpu8080 *cpu, uint16_t x);

void popPsw(cpu8080 *cpu);

// Here are the functions which deal with comparisons, and redircting code execution

void cmp(cpu8080 *cpu, uint8_t x);

void jmp(cpu8080 *cpu, uint8_t x, uint8_t y);

void call(cpu8080 *cpu, uint8_t x, uint8_t y);

void ret(cpu8080 *cpu);

void rst(cpu8080 *cpu, uint16_t x);

// Here are some functions to help with various parts of the code

uint8_t convertFlagsRegister(cpu8080 *cpu);

int uintConvert(uint8_t x, uint8_t y);

int charToInt(char * inp);

char *removeBeginning(char *inp);

int hexDecimaltoInt(char *arg);

void printTwoArgs(cpu8080 *cpu, int pc);

void printTwoArgsBack(cpu8080 *cpu, int pc);

void setSpecificFlagPsw(cpu8080 *cpu, int x, int cmp, bool *flag);

void setCode(cpu8080 *cpu, char *code, int size);

void scanBinary(char *fileName, cpu8080 *cpu);

long int getFileSize(FILE *fp);

// Here are some functions to help with the debugging functionallity

int isBreakPoint(cpu8080 *cpu);

breakpoints *findLastBreakpoint(breakpoints *breakpoint);

breakpoints *allocateBreakpoint(uint16_t x);

int checkBreakPoint(breakpoints * breakpt, uint16_t x);

// Here are functions which handle the debugger commands

void activateBreakpoint(cpu8080 *cpu, char *menuInput);

void createBreakpoint(cpu8080 *cpu, char *menuInput);

void deactivateBreakpoint(cpu8080 *cpu, char *menuInput);

void disassembleAll(cpu8080 *cpu);

void printHelp(void);

void printRegisters(cpu8080 *cpu);

void showBreakpoints(breakpoints *breakpoint, int n);

void printFlags(cpu8080 *cpu);

void jumpExec(cpu8080 *cpu, char *input);

void printMemoryRegions(cpu8080 *cpu);

void printRegister(cpu8080 *cpu, char *menuInput);

void resetExecution(cpu8080 *cpu);

cpu8080 *createCpu(void);

void destroyCpu(cpu8080 *cpu);

void stepInstruction(cpu8080 *cpu);

void toggleDisassExec(cpu8080 *cpu);

void dumpMemory(cpu8080 *cpu);

void examineMemory(cpu8080 *cpu, char *inp);

void resetExecution(cpu8080 *cpu);

void set(cpu8080 *cpu, char *inp);