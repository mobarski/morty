import re

def asm_to_cells(text, op_names):
	op_code = {n:i for i,n in enumerate(op_names)}
	macros = get_macros(text)
	text = strip_macros(text)
	tokens = tokenize(text)
	tokens = apply_macros(tokens, macros)
	cells = precompile(tokens, op_code)
	cells,labels = strip_labels(cells)
	cells = apply_labels(cells, labels)
	return cells

# TODO: relative jump for jz @+5 @-5
# TODO: relative jump target using stacks -> if if then then

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def get_macros(text):
	macros = {}
	raw_macros = re.findall('(?m)^\s*macro\s+.*',text)
	for m in raw_macros:
		tokens = re.split('\s+',m.strip())
		macros[tokens[1]] = tokens[2:]
	return macros

def strip_macros(text):
	text = re.sub('(?m)^\s*macro\s+.*','',text)
	return text

def tokenize(text):
	return re.split('\s+',text.strip())

def apply_macros(tokens, macros):
	out = []
	for t in tokens:
		if t in macros:
			for x in macros[t]:
				out += [x]
		else:
			out += [t]
	return out

def precompile(tokens, op_code):
	cells = []
	p = op_code['push']
	for t in tokens:
		c = op_code.get(t)
		if t in ['call','jz']:
			cells += [c]
		elif c is not None:
			cells += [c,0]
		elif t[0]=='@':
			cells += [t]
		elif t[-1]==':':
			cells += [t]
		else:
			cells += [int(t)]
	return cells

def strip_labels(cells):
	labels = {}
	out = []
	for c in cells:
		if type(c) is int:
			out += [c]
		elif c[-1]==':':
			name = c[:-1]
			labels[name] = len(out)
		else:
			out += [c]
		
	return out, labels

def apply_labels(cells, labels):
	out = []
	for c in cells:
		if type(c) is int:
			out += [c]
		else:
			name = c[1:]
			out += [labels[name]]
	return out

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
	op_names = "stop call ret jz push mul div".split(' ')
	cells = asm_to_cells(code, op_names)
	print(cells)
	