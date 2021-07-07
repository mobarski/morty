from asm import OPCODE

# generate REPL_SWITCH_BODY
if 1:
	for op in OPCODE:
		print(f"case {op.upper()}: goto OP_{op.upper()}; \\")

# generate indirect dispatch table
if 1:
	out = ['0']*256
	for op,code in OPCODE.items():
		out[code] = f'&&OP_{op.upper()}'
	values = ",".join(out)
	print("void* op_ptr[] = {",values,"};")
	
# op_name
if 1:
	out = ['"?"']*256
	for op,code in OPCODE.items():
		out[code] = f'"{op}"'
	values = ",".join(out)
	print("char* OP_NAME[] = {",values,"};")

# js OPS
if 1:
	print("\n// --- OPCODES (.js) --- ")
	for i,op in enumerate(OPCODE):
		if i%10==0:
			print("\b\b\nconst",end=' ')
		print(f"{op.upper()}={OPCODE[op]},",end=' ')
	print('\b\b')
