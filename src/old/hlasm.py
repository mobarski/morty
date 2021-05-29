import re

def hlasm_to_asm(code):
	out = []
	for line in code.split("\n"):
		indent = len(line)-len(line.lstrip())
		asm = []
		tokens = re.split('\s+',line)
		for t in tokens:
			if not t: continue
			try:
				v = int(t)
			except:
				v = None
			if v is not None:
				asm += [f'push {v}']
			elif '.' in t:
				asm += [t.replace('.',' ')]
			elif ':' in t or '@' in t:
				asm += [t]
			else:
				asm += [f'{t} 0']
		line_out = ' '*indent + '   '.join(asm)
		out += [line_out]
	return '\n'.join(out)


if __name__=="__main__":
	code = """
		0 jz @start
		mul2:
			dup add
			ret
		start:
			20 mul2
			1 1 add
			add 
			vminfo
			stop
	"""
	asm = hlasm_to_asm(code)
	print(asm)
