import re

def to_cells(text, op_names):
	op_code = {n:i for i,n in enumerate(op_names)}
	text = strip_comments(text)
	tokens = tokenize(text)
	cells = precompile(tokens, op_code)
	cells,labels = strip_labels(cells)
	# TODO detect errors
	cells = apply_labels(cells, labels)
	return cells


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def strip_comments(text):
	return re.sub('(?ms)[(].*?[)]', ' ', text)

def tokenize(text):
	return re.split('\s+',text.strip())

def precompile(tokens, op_code):
	cells = []
	for t in tokens:
		c = op_code.get(t)
		if c is not None:
			cells += [c]
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
		elif c[:2] in ['@+','@-']:
			out += [len(out) + int(c[1:])]
		elif c[0]=='@':
			name = c[1:]
			out += [labels[name]]
		else:
			raise Exception(f"unknown token: {c}")
	return out

# -----------------------------------------------------------------------------

if __name__=="__main__":
	code = """
		( this is a comment)
			vincr	3
			push	21
		
		(
			this
			is
			multi line
			comment
		)
		
			push	11
			jz		@start
		
		mul4:
			push 2
			push 2
			mul 0
			mul 0
			ret 0
		
		div4:
			push 4
			div 0
			ret 0
		
		start:
			push 44
		
			call @mul4
		
			stop 0
	"""
	op_names = "stop call ret jz push mul div vincr vget vset".split(' ')
	cells = to_cells(code, op_names)
	print(cells)
	