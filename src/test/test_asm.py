import sys; sys.path.append('../')
from asm import to_cells,OPCODE
from link import *

def test_asm(asm, optimize=False):
	print('asm:', asm)
	cells = to_cells(asm, OPCODE, do_optimize=optimize)
	print('before linking:',cells)
	cells = link(cells)
	print('after linking: ',cells)

# -----------------------------------------------------------------------------

if 0:
	asm = """
	push.0 jz.@start
	inc:
		push.1 add.0 ret.0
	start:
		push.2 call.@inc
	halt.0
	"""
	test_asm(asm)
	
if 0:
	asm = "push.0 jz.@[ push.1 add.0 push.@] push.2"
	test_asm(asm)

if 0:
	asm = "push.0 jz.@[ push.1 add.0 ]: push.2"
	test_asm(asm)

if 0:
	asm = "push.0 jz.@[ push.1 jz.@[ push.3 add.0 ]: add.0 ]: push.2"
	test_asm(asm)

if 0:
	asm = "push.2 times.@[ push.3 times.@[ add.0 loop.@] loop.@] push.4"
	test_asm(asm)

if 0:
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
	test_asm(asm)

# ERROR:
if 0:
	asm = "push.1   push.2   add.0   jz.@[   push.1   ret.0   qret.0   ]:   push.3"
	test_asm(asm, optimize=True)

if 1:
	#asm = "push.1 goto.@[ 11 22 33 push.@] push.@@" # @@ after @]
	asm = "push.1 goto.@[ 11 22 33 44 55 66 push.@@ push.@]" # @@ before @]
	test_asm(asm)
