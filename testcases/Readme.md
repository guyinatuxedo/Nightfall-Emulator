# Testcases

Here are the testcases that I wrote to test the functionallity of my emulator. All of the opcodes (with the exception of the ones that aren't documented). The first four blocks are dedicated to testing out the following commands:

```
block0:	instructions 0x00 - 0x3f
block1:	instructions 0x40 - 0x7f
block2:	instructions 0x80 - 0xbf
block3:	instructions 0xc0 - 0xff
``` 

All of the test cases for those blocks, can be found in the corresponding folders. Also all of those testcases have the assembly code stored in a `.asm` file.

The final block `block4` is dedicated to programs with more complicated code paths then what you would find in the first four blocks. This includes for loops, while loops, if then and switch statements, and a bit more branching / jumping. Also all of the testcases in this block also have a corresponding `.c` file which shows the C equivalent of the binary.
