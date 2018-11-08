import sys


def decompile(contents):
	i = 0
	limit = len(contents)
	while i < limit:
		if contents[i] == "\x00":
			print hex(i) + ": nop"
		elif contents[i] == "\x01":
			print hex(i) + ": lxi b "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2			
		elif contents[i] == "\x02":
			print hex(i) + ": stax b"
		elif contents[i] == "\x03":
			print hex(i) + ": inx b"
		elif contents[i] == "\x04":
			print hex(i) + ": inr b"
		elif contents[i] == "\x05":
			print hex(i) + ": dcr b"
		elif contents[i] == "\x06":
			print hex(i) + ": mvi b, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x07":
			print hex(i) + ": rlc"
		elif contents[i] == "\x09":
			print hex(i) + ": dad b"
		elif contents[i] == "\x0a":
			print hex(i) + ": ldax b"
		elif contents[i] == "\x0b":
			print hex(i) + ": inr c"
		elif contents[i] == "\x0c":
			print hex(i) + ": dcr c"
		elif contents[i] == "\x0d":
			print hex(i) + ": dcr c"
		elif contents[i] == "\x0e":
			print hex(i) + ": mvi c, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x0f":
			print hex(i) + ": rrc"


		elif contents[i] == "\x11":
			print hex(i) + ": lxi d "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2		
		elif contents[i] == "\x12":
			print hex(i) + ": stax d"
		elif contents[i] == "\x13":
			print hex(i) + ": inx d"
		elif contents[i] == "\x14":
			print hex(i) + ": inr d"
		elif contents[i] == "\x15":
			print hex(i) + ": dcr d"
		elif contents[i] == "\x16":
			print hex(i) + ": mvi d, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x17":
			print hex(i) + ": ral"
		elif contents[i] == "\x19":
			print hex(i) + ": dad d"
		elif contents[i] == "\x1a":
			print hex(i) + ": ldax d"
		elif contents[i] == "\x1b":
			print hex(i) + ": dcx d"
		elif contents[i] == "\x1c":
			print hex(i) + ": inr e"
		elif contents[i] == "\x1d":
			print hex(i) + ": dcr e"
		elif contents[i] == "\x1e":
			print hex(i) + ": mvi e, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x1f":
			print hex(i) + ": rar"


		elif contents[i] == "\x21":
			print hex(i) + ": lxi h "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2		
		elif contents[i] == "\x22":
			print hex(i) + ": shld "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\x23":
			print hex(i) + ": inx h"
		elif contents[i] == "\x24":
			print hex(i) + ": inr h"
		elif contents[i] == "\x25":
			print hex(i) + ": dcr h"
		elif contents[i] == "\x26":
			print hex(i) + ": mvi h, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x27":
			print hex(i) + ": daa"
		elif contents[i] == "\x29":
			print hex(i) + ": dad h"
		elif contents[i] == "\x2a":
			print hex(i) + ": lhld "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\x2b":
			print hex(i) + ": dcx h"
		elif contents[i] == "\x2c":
			print hex(i) + ": inr l"
		elif contents[i] == "\x2d":
			print hex(i) + ": dcr l"
		elif contents[i] == "\x2e":
			print hex(i) + ": mvi l, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x2f":
			print hex(i) + ": cma"

		elif contents[i] == "\x21":
			print hex(i) + ": lxi sp "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2	
		elif contents[i] == "\x32":
			print hex(i) + ": sta "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\x33":
			print hex(i) + ": inx sp"
		elif contents[i] == "\x34":
			print hex(i) + ": inr m"
		elif contents[i] == "\x35":
			print hex(i) + ": dcr m"
		elif contents[i] == "\x36":
			print hex(i) + ": mvi m, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x37":
			print hex(i) + ": stc"
		elif contents[i] == "\x39":
			print hex(i) + ": dad sp"
		elif contents[i] == "\x3a":
			print hex(i) + ": lda "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\x3b":
			print hex(i) + ": dcx sp"
		elif contents[i] == "\x3c":
			print hex(i) + ": inr a"
		elif contents[i] == "\x3d":
			print hex(i) + ": dcr a"
		elif contents[i] == "\x3e":
			print hex(i) + ": mvi a, " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\x3f":
			print hex(i) + ": cmc"

		elif contents[i] == "\x40":
			print hex(i) + ": mov b, b"
		elif contents[i] == "\x41":
			print hex(i) + ": mov b, c"
		elif contents[i] == "\x42":
			print hex(i) + ": mov b, d"
		elif contents[i] == "\x43":
			print hex(i) + ": mov b, e"
		elif contents[i] == "\x44":
			print hex(i) + ": mov b, h"
		elif contents[i] == "\x45":
			print hex(i) + ": mov b, l"
		elif contents[i] == "\x46":
			print hex(i) + ": mov b, m"
		elif contents[i] == "\x47":
			print hex(i) + ": mov b, a"
		elif contents[i] == "\x48":
			print hex(i) + ": mov c, b"
		elif contents[i] == "\x49":
			print hex(i) + ": mov c, c"
		elif contents[i] == "\x4a":
			print hex(i) + ": mov c, d"
		elif contents[i] == "\x4b":
			print hex(i) + ": mov c, e"
		elif contents[i] == "\x4c":
			print hex(i) + ": mov c, h"
		elif contents[i] == "\x4d":
			print hex(i) + ": mov c, l"
		elif contents[i] == "\x4e":
			print hex(i) + ": mov c, m"
		elif contents[i] == "\x4f":
			print hex(i) + ": mov c, a"



		elif contents[i] == "\x50":
			print hex(i) + ": mov d, b"
		elif contents[i] == "\x51":
			print hex(i) + ": mov d, c"
		elif contents[i] == "\x52":
			print hex(i) + ": mov d, d"
		elif contents[i] == "\x53":
			print hex(i) + ": mov d, e"
		elif contents[i] == "\x54":
			print hex(i) + ": mov d, h"
		elif contents[i] == "\x55":
			print hex(i) + ": mov d, l"
		elif contents[i] == "\x56":
			print hex(i) + ": mov d, m"
		elif contents[i] == "\x57":
			print hex(i) + ": mov d, a"
		elif contents[i] == "\x58":
			print hex(i) + ": mov e, b"
		elif contents[i] == "\x59":
			print hex(i) + ": mov e, c"
		elif contents[i] == "\x5a":
			print hex(i) + ": mov e, d"
		elif contents[i] == "\x5b":
			print hex(i) + ": mov e, e"
		elif contents[i] == "\x5c":
			print hex(i) + ": mov e, h"
		elif contents[i] == "\x5d":
			print hex(i) + ": mov e, l"
		elif contents[i] == "\x5e":
			print hex(i) + ": mov e, m"
		elif contents[i] == "\x5f":
			print hex(i) + ": mov e, a"


		elif contents[i] == "\x60":
			print hex(i) + ": mov h, b"
		elif contents[i] == "\x61":
			print hex(i) + ": mov h, c"
		elif contents[i] == "\x62":
			print hex(i) + ": mov h, d"
		elif contents[i] == "\x63":
			print hex(i) + ": mov h, e"
		elif contents[i] == "\x64":
			print hex(i) + ": mov h, h"
		elif contents[i] == "\x65":
			print hex(i) + ": mov h, l"
		elif contents[i] == "\x66":
			print hex(i) + ": mov h, m"
		elif contents[i] == "\x67":
			print hex(i) + ": mov h, a"
		elif contents[i] == "\x68":
			print hex(i) + ": mov l, b"
		elif contents[i] == "\x69":
			print hex(i) + ": mov l, c"
		elif contents[i] == "\x6a":
			print hex(i) + ": mov l, d"
		elif contents[i] == "\x6b":
			print hex(i) + ": mov l, e"
		elif contents[i] == "\x6c":
			print hex(i) + ": mov l, h"
		elif contents[i] == "\x6d":
			print hex(i) + ": mov l, l"
		elif contents[i] == "\x6e":
			print hex(i) + ": mov l, m"
		elif contents[i] == "\x6f":
			print hex(i) + ": mov l, a"


		elif contents[i] == "\x70":
			print hex(i) + ": mov m, b"
		elif contents[i] == "\x71":
			print hex(i) + ": mov m, c"
		elif contents[i] == "\x72":
			print hex(i) + ": mov m, d"
		elif contents[i] == "\x73":
			print hex(i) + ": mov m, e"
		elif contents[i] == "\x74":
			print hex(i) + ": mov m, h"
		elif contents[i] == "\x75":
			print hex(i) + ": mov m, l"
		elif contents[i] == "\x76":
			print hex(i) + ": hlt"
		elif contents[i] == "\x77":
			print hex(i) + ": mov m, a"
		elif contents[i] == "\x78":
			print hex(i) + ": mov a, b"
		elif contents[i] == "\x79":
			print hex(i) + ": mov a, c"
		elif contents[i] == "\x7a":
			print hex(i) + ": mov a, d"
		elif contents[i] == "\x7b":
			print hex(i) + ": mov a, e"
		elif contents[i] == "\x7c":
			print hex(i) + ": mov a, h"
		elif contents[i] == "\x7d":
			print hex(i) + ": mov a, l"
		elif contents[i] == "\x7e":
			print hex(i) + ": mov a, m"
		elif contents[i] == "\x7f":
			print hex(i) + ": mov a, a"


		elif contents[i] == "\x80":
			print hex(i) + ": add b"
		elif contents[i] == "\x81":
			print hex(i) + ": add c"
		elif contents[i] == "\x82":
			print hex(i) + ": add d"
		elif contents[i] == "\x83":
			print hex(i) + ": add e"
		elif contents[i] == "\x84":
			print hex(i) + ": add h"
		elif contents[i] == "\x85":
			print hex(i) + ": add l"
		elif contents[i] == "\x86":
			print hex(i) + ": add m"
		elif contents[i] == "\x87":
			print hex(i) + ": add a"
		elif contents[i] == "\x88":
			print hex(i) + ": adc b"
		elif contents[i] == "\x89":
			print hex(i) + ": adc c"
		elif contents[i] == "\x8a":
			print hex(i) + ": adc d"
		elif contents[i] == "\x8b":
			print hex(i) + ": adc e"
		elif contents[i] == "\x8c":
			print hex(i) + ": adc h"
		elif contents[i] == "\x8d":
			print hex(i) + ": adc l"
		elif contents[i] == "\x8e":
			print hex(i) + ": adc m"
		elif contents[i] == "\x8f":
			print hex(i) + ": adc a"





		elif contents[i] == "\x90":
			print hex(i) + ": sub b"
		elif contents[i] == "\x91":
			print hex(i) + ": sub c"
		elif contents[i] == "\x92":
			print hex(i) + ": sub d"
		elif contents[i] == "\x93":
			print hex(i) + ": sub e"
		elif contents[i] == "\x94":
			print hex(i) + ": sub h"
		elif contents[i] == "\x95":
			print hex(i) + ": sub l"
		elif contents[i] == "\x96":
			print hex(i) + ": sub m"
		elif contents[i] == "\x97":
			print hex(i) + ": sub a"
		elif contents[i] == "\x98":
			print hex(i) + ": sbb b"
		elif contents[i] == "\x99":
			print hex(i) + ": sbb c"
		elif contents[i] == "\x9a":
			print hex(i) + ": sbb d"
		elif contents[i] == "\x9b":
			print hex(i) + ": sbb e"
		elif contents[i] == "\x9c":
			print hex(i) + ": sbb h"
		elif contents[i] == "\x9d":
			print hex(i) + ": sbb l"
		elif contents[i] == "\x9e":
			print hex(i) + ": sbb m"
		elif contents[i] == "\x9f":
			print hex(i) + ": sbb a"




		elif contents[i] == "\xa0":
			print hex(i) + ": ana b"
		elif contents[i] == "\xa1":
			print hex(i) + ": ana c"
		elif contents[i] == "\xa2":
			print hex(i) + ": ana d"
		elif contents[i] == "\xa3":
			print hex(i) + ": ana e"
		elif contents[i] == "\xa4":
			print hex(i) + ": ana h"
		elif contents[i] == "\xa5":
			print hex(i) + ": ana l"
		elif contents[i] == "\xa6":
			print hex(i) + ": ana m"
		elif contents[i] == "\xa7":
			print hex(i) + ": ana a"
		elif contents[i] == "\xa8":
			print hex(i) + ": xra b"
		elif contents[i] == "\xa9":
			print hex(i) + ": xra c"
		elif contents[i] == "\xaa":
			print hex(i) + ": xra d"
		elif contents[i] == "\xab":
			print hex(i) + ": xra e"
		elif contents[i] == "\xac":
			print hex(i) + ": xra h"
		elif contents[i] == "\xad":
			print hex(i) + ": xra l"
		elif contents[i] == "\xae":
			print hex(i) + ": xra m"
		elif contents[i] == "\xaf":
			print hex(i) + ": xra a"


		elif contents[i] == "\xb0":
			print hex(i) + ": ora b"
		elif contents[i] == "\xb1":
			print hex(i) + ": ora c"
		elif contents[i] == "\xb2":
			print hex(i) + ": ora d"
		elif contents[i] == "\xb3":
			print hex(i) + ": ora e"
		elif contents[i] == "\xb4":
			print hex(i) + ": ora h"
		elif contents[i] == "\xb5":
			print hex(i) + ": ora l"
		elif contents[i] == "\xb6":
			print hex(i) + ": ora m"
		elif contents[i] == "\xb7":
			print hex(i) + ": ora a"
		elif contents[i] == "\xb8":
			print hex(i) + ": cmp b"
		elif contents[i] == "\xb9":
			print hex(i) + ": cmp c"
		elif contents[i] == "\xba":
			print hex(i) + ": cmp d"
		elif contents[i] == "\xbb":
			print hex(i) + ": cmp e"
		elif contents[i] == "\xbc":
			print hex(i) + ": cmp h"
		elif contents[i] == "\xbd":
			print hex(i) + ": cmp l"
		elif contents[i] == "\xbe":
			print hex(i) + ": cmp m"
		elif contents[i] == "\xbf":
			print hex(i) + ": cmp a"



		elif contents[i] == "\xc0":
			print hex(i) + ": rnz"
		elif contents[i] == "\xc1":
			print hex(i) + ": pop b"
		elif contents[i] == "\xc2":
			print hex(i) + ": jnz "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xc3":
			print hex(i) + ": jmp "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xc4":
			print hex(i) + ": cnz "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xc5":
			print hex(i) + ": push b"
		elif contents[i] == "\xc6":
			print hex(i) + ": adi " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xc7":
			print hex(i) + ": rst 0"
		elif contents[i] == "\xc8":
			print hex(i) + ": rz"
		elif contents[i] == "\xc9":
			print hex(i) + ": ret"
		elif contents[i] == "\xca":
			print hex(i) + ": jz "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xcc":
			print hex(i) + ": cz "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xcd":
			print hex(i) + ": call "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xce":
			print hex(i) + ": aci " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xcf":
			print hex(i) + ": rst 1"



		elif contents[i] == "\xd0":
			print hex(i) + ": rnc"
		elif contents[i] == "\xd1":
			print hex(i) + ": pop d"
		elif contents[i] == "\xd2":
			print hex(i) + ": jnc "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xd3":
			print hex(i) + ": out "  + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xd4":
			print hex(i) + ": cnc "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xd5":
			print hex(i) + ": push d"
		elif contents[i] == "\xd6":
			print hex(i) + ": sui " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xd7":
			print hex(i) + ": rst 2"
		elif contents[i] == "\xd8":
			print hex(i) + ": rc"
		elif contents[i] == "\xda":
			print hex(i) + ": jc "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xdc":
			print hex(i) + ": cc "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
                elif contents[i] == "\xdd":
                        print hex(i) + ": end " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xde":
			print hex(i) + ": sbi " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xdf":
			print hex(i) + ": rst 3"




		elif contents[i] == "\xe0":
			print hex(i) + ": rpo"
		elif contents[i] == "\xe1":
			print hex(i) + ": pop h"
		elif contents[i] == "\xe2":
			print hex(i) + ": jpo "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xe3":
			print hex(i) + ": xthl "
		elif contents[i] == "\xe4":
			print hex(i) + ": cpo "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xe5":
			print hex(i) + ": push h"
		elif contents[i] == "\xe6":
			print hex(i) + ": ani " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xe7":
			print hex(i) + ": rst 4"
		elif contents[i] == "\xe8":
			print hex(i) + ": rpe"
		elif contents[i] == "\xe9":
			print hex(i) + ": pchl"
		elif contents[i] == "\xea":
			print hex(i) + ": jpe "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xeb":
			print hex(i) + ": xchg"
		elif contents[i] == "\xec":
			print hex(i) + ": cpe "   + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xed":
			print hex(i) + ": xri " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xef":
			print hex(i) + ": rst 5"




		elif contents[i] == "\xf0":
			print hex(i) + ": rp"
		elif contents[i] == "\xf1":
			print hex(i) + ": pop psw"
		elif contents[i] == "\xf2":
			print hex(i) + ": jp "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xf3":
			print hex(i) + ": di"
		elif contents[i] == "\xf4":
			print hex(i) + ": cp "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xf5":
			print hex(i) + ": push psw"
		elif contents[i] == "\xf6":
			print hex(i) + ": ori " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xf7":
			print hex(i) + ": rst 6"
		elif contents[i] == "\xf8":
			print hex(i) + ": rm"
		elif contents[i] == "\xf9":
			print hex(i) + ": sphl"
		elif contents[i] == "\xfa":
			print hex(i) + ": jm "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xfb":
			print hex(i) + ": ei"
		elif contents[i] == "\xfc":
			print hex(i) + ": cm "  + hex(ord(contents[i + 2])) + hex(ord(contents[i + 1])).replace("0x", "")
			i += 2
		elif contents[i] == "\xfd":
			print hex(i) + ": cpi " + hex(ord(contents[i + 1]))
			i += 1
		elif contents[i] == "\xff":
			print hex(i) + ": rst 7"
		i += 1


if len(sys.argv) != 2:
	print "usage: python disassemble.py <file to decompile>"
	print len(sys.argv)
	exit()

fp = open(sys.argv[1], "r")
if fp.mode == "r":
	contents = fp.read()
	decompile(contents)
