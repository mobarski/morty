import re

def asm_to_cells(text, op_names, arg_ops=[]):
	op_code = {n:i for i,n in enumerate(op_names)}
	macros = get_macros(text)
	text = strip_macros(text)
	tokens = tokenize(text)
	tokens = apply_macros(tokens, macros)
	cells = precompile(tokens, op_code, arg_ops)
	cells,labels = strip_labels(cells)
	cells = apply_labels(cells, labels)
	return cells

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

# TODO how to extend
def precompile(tokens, op_code, arg_ops):
	cells = []
	p = op_code['push']
	prev_t = ''
	for t in tokens:
		c = op_code.get(t)
		if t in ['call','jz']:
			cells += [c]
		elif t[0]=='@':
			cells += [t]
		elif t[-1]==':':
			cells += [t]
		elif t in arg_ops:
			cells += [c]
		elif prev_t in arg_ops:
			cells += [int(t)]
		elif c is not None: # must be before else
			cells += [c,0]
		else:
			cells += [p,int(t)]
		prev_t = t
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
		elif c[:2] in ['@+','@-']:
			out += [int(c[1:])]
		elif c[:2] in ['@@']:
			name = c[2:]
			out += [labels[name] - len(out)]
		else:
			name = c[1:]
			out += [labels[name]]
	return out

if __name__=="__main__":
	code = """
		vincr 3
		macro 2+ 2 add
		21
		macro 2* 2 mul
		macro 2/ 2 div
		
		11 jz @@start
		
		mul4:
			2* 2* ret
		
		div4:
			2/ 2/ ret
		
		start:
			44
		
		call @mul4
		
		stop
	"""
	op_names = "stop call ret jz push mul div vincr vget vset".split(' ')
	cells = asm_to_cells(code, op_names, arg_ops=['vincr','vget','vset'])
	print(cells)
	