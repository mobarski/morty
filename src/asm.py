import re
from array import array
import argparse
import sys

import peephole

# TODO: comments as tokens similar to labels ??? NO -> optimization
# TODO: map token to file @ line

def to_cells(text, op_code, do_optimize=False, do_debug=False):
	text          = strip_comments(text)
	tokens        = tokenize(text)
	if do_optimize: # TODO: multi-pass optimization
		tokens    = optimize(tokens, peephole.rules)
		if do_debug:
			print(" ".join(tokens), file=sys.stderr)
	tokens        = split_tokens(tokens)
	pos_by_label  = detect_labels(tokens)
	tokens        = apply_labels(tokens, pos_by_label)
	cells         = compile(tokens, op_code)
	return cells

def cells_to_text(cells, per_line=10, sep='\t'):
	compiled = []
	for i in range(0, len(cells), per_line):
		line = sep.join([str(x) for x in cells[i:i+per_line]])
		compiled += [line]
	return "\n".join(compiled)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def strip_comments(text):
	text = re.sub('(?ms)[(]{2}.*?[)]{2}', ' ', text) # multiline comments (( ... ))
	text = re.sub('\s[(].*?[)]', ' ', text) # inline comments ( ... )
	text = re.sub('\s[#].*', ' ', text) # line comments -> code # comment
	return text

def tokenize(text):
	return re.split('\s+',text.strip())

def split_tokens(tokens):
	out = []
	for t in tokens:
		# TODO error detection
		# TODO vs float
		if t[0]=='.': # allow labels starting with the dot
			out += [t]
			continue
		for c in t.split('.',1):
			out += [c]
	return out

def detect_labels(tokens):
	""
	pos = 0
	pos_by_label = {}
	#print(list(enumerate(tokens)),file=sys.stderr) # XXX
	for i,t in enumerate(tokens):
		if t[-1]==':':
			label = t[:-1]
			pos_by_label[label] = pos
		else:
			pos += 1
	return pos_by_label

def apply_labels(tokens, pos_by_label):
	""
	out = []
	stack = []
	pos = 0
	#print(tokens) # XXX
	for t in tokens:
		# if not t: # XXX
			# print(out)
			# break
		if t=='@[':
			stack.append((pos,len(out)))
			out += [t]
			pos += 1
		elif t=='@]':
			target,i = stack.pop()
			out[i] = str(pos-1)
			out += [str(target+1)]
			pos += 1
		elif t==']:':
			target,i = stack.pop()
			out[i] = str(pos)
			out += [t]
		elif t[0]=='@':
			label = t[1:]
			p = pos_by_label[label]
			out += [str(p)]
			pos += 1
		elif t[:2] in ['@+','@-']: # NOT USED
			p = pos + int(arg[1:])
			out += [str(p)]
			pos += 1
		elif t[-1]==":":
			out += [t]
		else:
			out += [t]
			pos += 1
	return out

def compile(tokens, op_code):
	""
	cells = []
	#print(tokens) # XXX
	for i,t in enumerate(tokens):
		c = op_code.get(t)
		if c is not None:
			cells += [c]
		elif t[-1] == ':':
			pass
		else:
			try:
				cells += [int(t)]
			except ValueError:
				N = 6
				print('ERROR',i, tokens[max(0,i-N):i],t,tokens[i+1:i+1+N], file=sys.stderr) # TODO: error reporing
	return cells


def optimize(tokens,rules):
	"one pass of the peephole optimizer"
	out = []
	total_match_cnt = 0
	i = 0
	while i<len(tokens):
		for n,match_fun,replace_fun in rules:
			if i+n>=len(tokens): continue
			old_out = tokens[i:i+n]
			match = match_fun(*old_out)
			if match:
				new_out = replace_fun(*old_out)
				#print("OPTIMIZATION",old_out,"->",new_out) # XXX
				out.extend(new_out)
				i += n
				total_match_cnt += 1
				break
		else:
			out += [tokens[i]]
			i += 1
	return out

# -----------------------------------------------------------------------------

OPS = [
	'HALT',
	'NOP','PUSHA',
	'PUSH','DUP','DROP','SWAP','STOR','RTOS',
	'ADD','MUL','DIV','SUB',
	'AND','OR','XOR','NZ',
	'CALL','RET','JZ',
	'QCALL','QRET',
	'GET','SET','ALLOT',
	'MOD','SHL','SHR','USHR','INVERT','NEGATE','ABS',
	'LE','LT','GE','GT','EQ','NE',
	'XLE','XLT','XGE','XGT','XEQ','XNE',
	'ROT','OVER','UNROT',
	'VGET','VSET','RADD','GGET','GSET',
	'EMIT','DOT','ECHO',
	'IF','IFELSE','GOTO',
	'LAMBDA',
	'MIN','MAX','PICK',
	'GTZ','EQZ','LTZ',
	'TIMES','LOOP','FOR','RSUB','RGET','BEGIN','BREAK','CONTINUE',
	'ADDI','MULI','ADDGET','ADDSET','IADDGET','IADDSET',
	'LEI','LTI','GEI','GTI','EQI','NEI',
	'IOGET','IOSET',
	'VMINFO'
]
OPCODE = {n.lower():i for i,n in enumerate(OPS)}
OPCODE['vminfo'] = 255

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Compile MortyVM assembler code into binary cells')
	parser.add_argument('-o',  metavar='path', type=str, help='output path (default: stdout)')
	parser.add_argument('-i',  metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-d',  action='store_true', help='debug')
	parser.add_argument('-O',  action='store_true', help='optimize')
	args = parser.parse_args()

	if args.i:
		code = open(args.i,'r').read()
	else:
		code = sys.stdin.read()		

	cells = to_cells(code, OPCODE, do_optimize=args.O, do_debug=args.d)
	text = cells_to_text(cells)
	if args.d:
		print(text, file=sys.stderr)
		print(f"cells: {len(cells)}", file=sys.stderr)
	if args.o:
		open(args.o,'w').write(text)
	else:
		print(text)
