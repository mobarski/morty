import re
import sys
import argparse
from asm import OPCODE

def to_asm(text):
	tokens = tokenize(text)
	ops = compile(tokens)
	asm = ''.join([op['asm'] for op in ops])
	return asm

# ------------------------------------------------------------------------------

def tokenize(text):
	return re.findall("""(?xms)
			  [(][(].*?[)][)]  # multi-line comment
			| [(].*?[)]        # inline comment
			| [(].*?\n         # line comment
			| [^ \t\r\n]+      # "normal" token
			| \s+              # whitespace
		""",text)

ops = OPCODE.keys()
def compile(tokens):
	out = []
	functions = []
	local = []
	glob = []
	i = 0
	while i<len(tokens):
		t = tokens[i]
		i += 1
		if t[0] in '\r\n\t (':
			asm = t
		elif t=='def':
			name = tokens[i]
			functions += [name]
			asm = f'goto.@[ {name}:'
			i += 1
		elif t=='end':
			local = []
			asm = 'ret.0 ]:'
		elif t=='times':
			asm = 'times.@['
		elif t=='loop':
			asm = 'loop.@]'
		elif t=='then':
			asm = 'jz.@['
		elif t=='do':
			asm = ']:'
		# names
		elif t[0]==':':
			name = t[1:]
			if name not in local:
				local += [name]
				asm = f'stor.0'
			else:
				idx = 1+local.index(name)
				asm = f'vset.{idx}'
		elif t[0]=='$':
			name = t[1:]
			if name not in glob:
				glob += [name]
			idx = glob.index(name)
			asm = f'gset.{idx}'
		elif t in local:
			idx = 1+local.index(t)
			asm = f'vget.{idx}'
		elif t in glob:
			idx = glob.index(t)
			asm = f'gget.{idx}'
		elif t in functions:
			asm = f'call.@{t}'
		elif t in ops:
			asm = f'{t}.0'
		# 
		elif is_int(t):
			x = int(t)
			asm = f'push.{x}'
		else:
			# TODO: verify
			asm = t
		out += [dict(asm=asm)]
	return out

def is_int(x):
	try:
		int(x)
		return True
	except:
		return False

# ------------------------------------------------------------------------------

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Compile Morty source code into MortyVM assembler')
	parser.add_argument('-o',  metavar='path', type=str, help='output path')
	parser.add_argument('-i',  metavar='path', type=str, help='input path (default: stdin)')
	args = parser.parse_args()

	if args.i:
		code = open(args.i,'r').read()
	else:
		code = sys.stdin.read()

	asm = to_asm(code)
	
	if args.o:
		with open(args.o,'w') as f:
			print(asm,file=f)
	else:
		print(asm)
