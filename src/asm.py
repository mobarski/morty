import re
from array import array
import argparse
import sys

# TODO: map token to file @ line

def to_cells(text, op_code):
	text          = strip_comments(text)
	tokens        = tokenize(text)
	tokens,labels = strip_labels(tokens)
	tokens        = apply_labels(tokens, labels)
	cells         = compile(tokens, op_code)
	return cells

def to_binary_file(filename, text, op_code):
	cells = to_cells(code, op_code)
	compiled = array("i", cells).tobytes()
	open(filename,'wb').write(compiled)
	return cells

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def strip_comments(text):
	return re.sub('\s[(].*', ' ', text)

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

OPS = [
	'STOP',
	'PUSH','DUP','DROP','SWAP','STOR','RTOS',
	'ADD','MUL','DIV','SUB',
	'AND','OR','XOR','NZ',
	'CALL','RET','JZ',
	'QCALL','QRET',
	'GET','SET','ALLOT',
	'MOD','SHL','SHR','USHR','MIN','MAX',
	'LE','LT','GE','GT','EQ','NE',
	'ROT','OVER',
	'VGET','VSET','RADD',
	'EMIT','DOT','ECHO',
	'VMINFO'
]
OPCODE = {n.lower():i for i,n in enumerate(OPS)}
OPCODE['vminfo'] = 777

if __name__=="__main__":

	parser = argparse.ArgumentParser(description='Compile MortyVM assembler code into binary cells')
	parser.add_argument('-o', metavar='path', type=str, help='output path')
	parser.add_argument('-i', metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-d', action='store_true', help='debug')
	args = parser.parse_args()

	if args.i:
		code = open(args.i,'r').read()
	else:
		code = sys.stdin.read()		

	cells = to_cells(code, OPCODE)
	if args.d:
		print(cells)
	if args.o:
		to_binary_file(args.o, code, OPCODE)
