0x0: mvi b, 0xff
0x2: mvi c, 0xff
0x4: push b
0x5: pop psw
0x6: cnz 0x00
0x9: cnc 0x00
0xc: cpo 0x00
0xf: cp 0x00
0x12: call 0x017
0x15: end 0x66
0x17: nop
0x18: nop
0x19: nop
0x1a: rnz
0x1b: rnc
0x1c: rpo
0x1d: rp
0x1e: ret