import re

# TODO map token to file @ line

def to_cells(text, op_names):
	op_code = {n:i for i,n in enumerate(op_names)}
	text          = strip_comments(text)
	tokens        = tokenize(text)
	tokens,labels = strip_labels(tokens)
	tokens        = apply_labels(tokens, labels)
	cells         = compile(tokens, op_code)
	return cells

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def strip_comments(text):
	# TODO handle character cells '( and ')
	# TODO handle strings (FUTURE)
	return re.sub('(?ms)[(].*?[)]', ' ', text)

def tokenize(text):
	return re.split('\s+',text.strip())

def strip_labels(tokens):
	labels = {}
	out = []
	for t in tokens:
		if t[-1]==':':
			name = t[:-1]
			labels[name] = len(out)
		else:
			out += [t]
		
	return out, labels

def apply_labels(tokens, labels):
	out = []
	for t in tokens:
		if t[0]=='@':
			name = t[1:]
			out += [labels[name]]
		elif t[:2] in ['@+','@-']:
			out += [len(out) + int(t[1:])]
		elif t[0]=="'":
			out += [ord(t[1:-1])]
		else:
			out += [t]
	return out

def compile(tokens, op_code):
	cells = []
	for i,t in enumerate(tokens):
		c = op_code.get(t)
		if c is not None:
			cells += [c]
		else:
			if i%2==0:
				N = 5
				info = ' '.join([str(x) for x in tokens[i-N:i]]) + f" >>>{t}<<< " + ' '.join([str(x) for x in tokens[i+1:i+N+1]])
				print(f"ERROR: invalid token '{t}' at position {i}:\n{info}\n")
				raise Exception('invalid syntax')
			cells += [int(t)]
	return cells

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
		
			push '*'
			
			emit 0
		
			stop 0
	"""
	op_names = "stop call ret jz push mul div vincr vget vset emit".split(' ')
	cells = to_cells(code, op_names)
	print(cells)
	