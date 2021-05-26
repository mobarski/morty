import vmasm

code = """
	macro 2+ 2 add
	21
	macro 2* 2 mul
	macro 2/ 2 div
	
	0 jz @start
	
	mul4:
		2* 2* ret
	
	div4:
		2/ 2/ ret
	
	start:
	
	call @mul4
	
	stop
"""
op_names = "stop call ret jz push mul div".split(' ')
cells = vmasm.asm_to_cells(code, op_names)
print(cells)