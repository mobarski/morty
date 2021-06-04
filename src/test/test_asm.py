import sys; sys.path.append('../')
from asm import *

if 0:
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
if 0:
	hla = """
	0 jz.@start
	inc:
		1 add ret
	start:
		2 :inc
	stop
	"""
	asm = hlasm_to_asm(hla)
	cells = to_cells(asm, OPCODE)
	print(cells)
	print(asm)

if 0:
	asm = "push.0 jz.@[ push.1 add.0 push.@] push.2"
	cells = to_cells(asm, OPCODE)
	print(cells)

if 1:
	asm = "push.0 jz.@[ push.1 add.0 ]: push.2"
	cells = to_cells(asm, OPCODE)
	print(cells)

if 1:
	asm = "push.0 jz.@[ push.1 jz.@[ push.3 add.0 ]: add.0 ]: push.2"
	cells = to_cells(asm, OPCODE)
	print(cells)

if 0:
	asm = "push.2 times.@[ push.3 times.@[ add.0 loop.@] loop.@] push.4"
	cells = to_cells(asm, OPCODE)
	print(cells)

if 1:
	asm = """
		push.0   jz.@start
		aaa:
		loops:
		 stor.0
		 push.0
		 vget.1   times.@[
		  vget.1   times.@[
		   vget.1   times.@[
			vget.1   times.@[
			 vget.1   times.@[
			  vget.1   times.@[
			   push.1   add.0
			  loop.@]
			 loop.@]
			loop.@]
		   loop.@]
		  loop.@]
		 loop.@]
		ret.0
		
		start:
		 push.28   call.@loops   vminfo.0
	"""
	cells = to_cells(asm, OPCODE)
	print(cells)
