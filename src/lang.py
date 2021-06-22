import re
import argparse
from asm import OPCODE

def to_asm(text):
	text = strip_comments(text)
	tokens = tokenize(text)
	ops = compile(tokens)
	asm = ' '.join([op['asm'] for op in ops])
	return asm

# ------------------------------------------------------------------------------

def strip_comments(text):
	text = re.sub('(?ms)[(]{2}.*?[)]{2}', ' ', text) # multiline comments (( ... ))
	text = re.sub('\s[(].*?[)]', ' ', text)
	text = re.sub('\s[(].*', ' ', text)
	return text

# TODO: preserve leading whitespace (indent)
def tokenize(text):
	return re.findall('[^ \t\r\n]+|\n',text.strip())

ops = OPCODE.keys()
def compile(tokens):
	out = []
	functions = []
	local = []
	i = 0
	while i<len(tokens):
		t = tokens[i]
		i += 1
		if t=='\n':
			out += [dict(asm='\n')]
		elif t=='def':
			name = tokens[i]
			functions += [name]
			out += [dict(asm=f'goto.@[ {name}:')]
			i += 1
		elif t=='end':
			local = []
			out += [dict(asm='ret.0 ]:')]
		elif t=='times':
			out += [dict(asm='times.@[')]
		elif t=='loop':
			out += [dict(asm='loop.@]')]
		elif t=='then':
			out += [dict(asm='jz.@[')]
		elif t=='do':
			out += [dict(asm=']:')]
		elif t[0]==':':
			# TODO: reserve frame cells right after "def"
			name = t[1:]
			if name not in local:
				local += [name]
				out += [dict(asm=f'stor.0')]
			else:
				fi = 1+local.index(name)
				out += [dict(asm=f'vset.{fi}')]
		elif t in local:
			fi = 1+local.index(t)
			out += [dict(asm=f'vget.{fi}')]
		elif t in functions:
			out += [dict(asm=f'call.@{t}')]
		elif t in ops:
			out += [dict(asm=f'{t}.0')]
		elif is_int(t):
			x = int(t)
			out += [dict(asm=f'push.{x}')]
		else:
			# TODO: verify
			out += [dict(asm=t)]
	return out

def is_int(x):
	try:
		int(x)
		return True
	except:
		return False

# ------------------------------------------------------------------------------

def xxx_test():
	x = to_asm("""
		def foo ( a -- b )
			2 mul
		end
		def bar ( -- )
			10 times
				1 dot
			loop
		end
		def baz ( a -- )
			5 lt then
				2 dot
			do
		end
		def foobar
			:a :b :c
			1 a mul
			2 b add
			3 c div
		end
		20 foo 2 add
		dot
	""")
	print(x)

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
