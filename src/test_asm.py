import vmasm
import re

# vmasm.OP_NAMES = re.findall('\w+',"""
	# call ret jz push mul div stop
# """)

if __name__=="__main__":
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
	cells = vmasm.asm_to_cells(code)
	print(cells)