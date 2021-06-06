import re
from array import array
import argparse
import sys

import peephole

# TODO: comments as tokens similar to labels ??? NO -> optimization
# TODO: map token to file @ line

def to_cells(text, op_code, do_optimize=False):
	text          = strip_comments(text)
	tokens        = tokenize(text)
	if do_optimize: # TODO: multi-pass optimization
		tokens    = optimize(tokens, peephole.rules)
	tokens        = split_tokens(tokens)
	pos_by_label  = detect_labels(tokens)
	tokens        = apply_labels(tokens, pos_by_label)
	cells         = compile(tokens, op_code)
	return cells

def to_binary_file(filename, text, op_code):
	cells = to_cells(code, op_code)
	compiled = array("i", cells).tobytes()
	open(filename,'wb').write(compiled)
	return cells

def to_text_file(filename, text, op_code):
	cells = to_cells(code, op_code)
	compiled = []
	PER_LINE = 10 # TODO param
	for i in range(0, len(cells), PER_LINE):
		line = "\t".join([str(x) for x in cells[i:i+PER_LINE]])
		compiled += [line]
	open(filename,'w').write("\n".join(compiled))
	return cells	

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

def strip_comments(text):
	text = re.sub('(?ms)[(]{2}.*?[)]{2}', ' ', text) # multiline comments (( ... ))
	text = re.sub('\s[(].*?[)]', ' ', text)
	text = re.sub('\s[(].*', ' ', text)
	return text

def tokenize(text):
	return re.split('\s+',text.strip())

def split_tokens(tokens):
	out = []
	for t in tokens:
		# TODO partition
		# TODO error detection
		# TODO vs float
		for c in t.split('.'):
			out += [c]
	return out

def detect_labels(tokens):
	pos = 0
	pos_by_label = {}
	for i,t in enumerate(tokens):
		if t[-1]==':':
			label = t[:-1]
			pos_by_label[label] = pos
		else:
			pos += 1
	return pos_by_label

def apply_labels(tokens, pos_by_label):
	out = []
	stack = []
	pos = 0
	#print(tokens) # XXX
	for t in tokens:
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
		elif t[:2] in ['@+','@-']:
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
				print('ERROR',i, tokens[max(0,i-N):i],t,tokens[i+1:i+1+N]) # TODO: error reporing
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


def hlasm_to_asm(code):
	"""Morty High Level Assebler
	
	Tranformations:
		number   -> push.number
		'c'      -> push.char_as_number
		label:   -> label:
		:label   -> call.@label
		op.@xxx  -> op.@xxx
		op       -> op.0
	"""
	code = strip_comments(code)
	
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
				asm += [f'push.{v}']
			elif t[0]=="'":
				v = ord(t[1:2])
				asm += [f'push.{v}']
			elif '.' in t:
				asm += [t]
			elif t[-1]==':':
				asm += [t]
			elif t[0]==':':
				v = t[1:]
				asm += [f'call.@{v}']
			else:
				asm += [f'{t}.0']
		line_out = ' '*indent + '   '.join(asm)
		out += [line_out]
	return '\n'.join(out)

# -----------------------------------------------------------------------------

OPS = [
	'STOP',
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
	'VGET','VSET','RADD',
	'EMIT','DOT','ECHO',
	'IF','IFELSE','GOTO',
	'LAMBDA',
	'MIN','MAX','PICK',
	'GTZ','EQZ','LTZ',
	'TIMES','LOOP','RSUB','RGET',
	'IOGET','IOSET',
	'VMINFO'
]
OPCODE = {n.lower():i for i,n in enumerate(OPS)}
OPCODE['vminfo'] = 255

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Compile MortyVM assembler code into binary cells')
	parser.add_argument('-o',  metavar='path', type=str, help='output path')
	parser.add_argument('-i',  metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-hl', action='store_true', help='treat input as high level assembler')
	parser.add_argument('-d',  action='store_true', help='debug')
	parser.add_argument('-t',  action='store_true', help='text output')
	parser.add_argument('-O',  action='store_true', help='optimize')
	args = parser.parse_args()

	if args.i:
		code = open(args.i,'r').read()
	else:
		code = sys.stdin.read()		

	if args.hl:
		code = hlasm_to_asm(code)
		if args.d:
			print(code)

	cells = to_cells(code, OPCODE)
	if args.d:
		print(cells)
		print(f"cells: {len(cells)}")
	if args.o:
		if args.t:
			to_text_file(args.o, code, OPCODE)
		else:
			to_binary_file(args.o, code, OPCODE)
