0x0: mvi a, 0x66
0x2: mvi b, 0x78
0x4: cpi, 0x66
0x6: jz, 0x0010
0x9: cmp b
0xa: jz, 0x0024
0xd: jmp, 0x0038
0x10: cmp b
0x11: jnz, 0x001c
0x14: mvi a, 0x15
0x16: call, 0x004f
0x19: jmp, 0x004c
0x1c: mvi a, 0x93
0x1e: call, 0x004f
0x21: jmp, 0x004c
0x24: cmp b
0x25: jz, 0x0030
0x28: mvi a, 0x57
0x2a: call, 0x004f
0x2d: jmp, 0x004c
0x30: mvi a, 0x28
0x32: call, 0x004f
0x35: jmp, 0x004c
0x38: cmp d
0x39: jz, 0x0044
0x3c: mvi a, 0x35
0x3e: call, 0x004f
0x41: jmp, 0x004c
0x44: mvi a, 0x79
0x46: call, 0x004f
0x49: jmp, 0x004c
0x4c: nop
0x4d: end 0x66
0x4f: add b
0x50: ret