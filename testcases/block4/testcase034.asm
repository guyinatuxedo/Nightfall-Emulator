0x0: mvi a, 0x0
0x2: mvi b, 0x25
0x4: mvi c, 0x3
0x6: cpi, 0xa
0x8: jnc, 0x0023
0xb: cpi, 0x7
0xd: jnz, 0x0012
0x10: adi, 0x1
0x12: cmp b
0x13: jnz, 0x0018
0x16: mvi a, 0x6
0x18: cmp c
0x19: jnz, 0x001e
0x1c: mvi a, 0x23
0x1e: adi, 0x1
0x20: jmp, 0x0006
0x23: nop
0x24: end 0x66
