import sys; sys.path.append('../')
from asm import *

if 1:
	asm = """
	push.0 jz.@start
	inc:
		push.1 add.0 ret.0
	start:
		push.2 call.@inc
	stop.0
	"""
	cells = to_cells(asm, OPCODE)
	print(cells)
if 1:
	hla = """
	0 jz.@start
	inc:
		1 add ret
	start:
		2 call.@inc
	stop
	"""
	asm = hlasm_to_asm(hla)
	cells = to_cells(asm, OPCODE)
	print(cells)
	print(asm)
