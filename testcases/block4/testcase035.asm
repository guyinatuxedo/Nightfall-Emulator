0x0: mvi a, 0x0
0x2: mvi b, 0x25
0x4: mvi c, 0x3
0x6: cpi, 0xa
0x8: jnc, 0x0025
0xb: cpi, 0x7
0xd: jnz, 0x0013
0x10: call, 0x0028
0x13: cmp b
0x14: jnz, 0x001a
0x17: call, 0x002a
0x1a: cmp c
0x1b: jnz, 0x0021
0x1e: call, 0x002d
0x21: inr a
0x22: jmp, 0x0006
0x25: nop
0x26: end 0x66
0x28: inr a
0x29: ret
0x2a: mvi a, 0x6
0x2c: ret
0x2d: mvi a, 0x23
0x2f: ret
