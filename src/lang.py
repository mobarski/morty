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
			| [#].*?\n         # line comment
			| [^ \t\r\n\[\]]+  # "normal" token
			| [\[\]]           # separators
			| \s+              # whitespace
		""",text)

ops = OPCODE.keys()
def compile(tokens):
	out = []
	mode = 'normal'
	functions = []
	local = []
	glob = []
	struct = {}
	struct_name = ''
	i = 0
	while i<len(tokens):
		t = tokens[i]
		i += 1
		
		# MODE: raw
		if mode == 'raw':
			if t == '}':
				mode = 'normal'
				asm = 'push.@@ push.@]'
			else:
				asm = t
			out += [dict(asm=asm)]
			continue
		
		# MODE: struct
		if mode == 'struct':
			if '\n' in t:
				mode = 'normal'
				asm = t
			elif t[0]=='.':
				struct[struct_name] += [t]
				asm = ''
			else:
				asm = t
			out += [dict(asm=asm)]
			continue
		
		# MODE: normal
		if t[0] in '\r\n\t (#':
			asm = t
		elif t[-1]==':':
			asm = t # LABEL
		elif '.' in t:
			asm = t # VM ASM op
		elif t[0] in '[]':
			asm = f' ({t}) '
		elif t=='import':
			name = tokens[i+1] # skip whitespace
			i += 2 # whitespace + name
			tokens[i:i] = get_module_tokens(name)
			asm = ''
		# sugar
		elif t in ['if']:
			asm = ''
		# functions
		elif t=='def':
			name = tokens[i+1] # skip whitespace
			functions += [name]
			asm = f'goto.@[ {name}:'
			i += 2 # whitespace + name
		elif t=='end':
			local = []
			asm = 'ret.0 ]:'
		# loops
		elif t=='times':
			asm = 'times.@['
		elif t=='for':
			asm = 'for.@['
		elif t=='loop':
			asm = 'loop.@]'
		elif t=='then':
			asm = 'jz.@['
		elif t=='do':
			asm = ']:'
		elif t=='i':
			asm = 'rget.0'
		# arrays # TODO: stdlib as macro
		elif t=='.get':
			asm = 'add.0 get.0'
		elif t=='.set':
			asm = 'add.0 set.0'
		# names
		elif t[0]==':':
			name = t[1:]
			if name not in local:
				local += [name]
				asm = 'stor.0'
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
			asm = f"call.@{t}"
		elif t[0]=='&':
			asm = f"push.@{t[1:]}"
		# mode change
		elif t == '{':
			asm = 'goto.@['
			mode = 'raw'
		elif t == 'struct':
			mode = 'struct'
			struct_name = tokens[i+1] # skip whitespace
			struct[struct_name] = []
			i += 2 # whitespace + name
			asm = f'( struct {struct_name} ... )'
		#
		elif t in ops:
			asm = f'{t}.0'
		elif is_int(t):
			x = int(t)
			asm = f'push.{x}'
		else: # call unknown function
			#asm = t # OLD
			asm = f"call.@{t}" # TEST: regression
		out += [dict(asm=asm)]
	return out

def is_int(x):
	try:
		int(x)
		return True
	except:
		return False

def get_module_tokens(name):
	code = open(f'morty/{name}.morty').read()
	return tokenize(code)

# ------------------------------------------------------------------------------

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Compile Morty source code into MortyVM assembler')
	parser.add_argument('-o',  metavar='path', type=str, help='output path (default: stdout)')
	parser.add_argument('-f',  metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-d',  action='store_true', help='debug')
	args = parser.parse_args()

	if args.f:
		code = open(args.f,'r').read()
	else:
		code = sys.stdin.read()

	asm = to_asm(code)
	
	if args.d:
		print(asm, file=sys.stderr)
	if args.o:
		with open(args.o,'w') as f:
			print(asm,file=f)
	else:
		print(asm)
