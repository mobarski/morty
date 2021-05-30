from asm import OPCODE

# generate REPL_SWITCH_BODY
for op in OPCODE:
	print(f"case {op.upper()}: goto OP_{op.upper()}; \\")
