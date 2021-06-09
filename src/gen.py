from asm import OPCODE

# generate REPL_SWITCH_BODY
if 0:
	for op in OPCODE:
		print(f"case {op.upper()}: goto OP_{op.upper()}; \\")

if 1:
	out = ['0']*256
	for op,code in OPCODE.items():
		out[code] = f'&&OP_{op.upper()}'
	values = ",".join(out)
	print("void* op_ptr[] = {",values,"};")
	
