# Source code

Here is the source code for the emulator. Here is how you can compile the source code:

```
$	git clone git@github.com:guyinatuxedo/Nightfall-Emulator.git
$	cd Nightfall-Emulator/src/
$	make
gcc Nightfall.c emulate.c debugCmds.c disass.c flagSet.c math.c stack.c utills.c debugUtils.c codeExec.c mov.c -o Nightfall
```
