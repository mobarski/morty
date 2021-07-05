import sys
import argparse
import re

# -----------------------------------------------------------------------------

def link(tokens):
	pos_by_label = get_pos_by_label(tokens)
	tokens = apply_labels(tokens, pos_by_label)
	return tokens

def tokenize(code):
	tokens = re.split('\s+', code.strip())
	return tokens

def serialize(tokens):
	text = '\t'.join([str(t) for t in tokens])
	return text

# -----------------------------------------------------------------------------

def is_linker_token(t):
	try:
		int(t)
		return False
	except:
		return True

def get_pos_by_label(tokens):
	pos = 0
	pos_by_label = {}
	#print(list(enumerate(tokens)),file=sys.stderr) # XXX
	for i,t in enumerate(tokens):
		if type(t) is int:
			pos += 1
		elif t[-1]==':':
			label = t[:-1]
			pos_by_label[label] = pos
		else:
			pos += 1
	return pos_by_label

def apply_labels(tokens, pos_by_label):
	"""return tokens with labels references replaced by addresses
	notation:
	- x: -> a label (nothing to write)
	- @x -> write the address of the label
	- @[ -> push current address on the compiler stack, write placeholder
	- @] -> pop address from the assembler stack, write current address there, and write that address here
	- @@ -> write push.len, where len is offset between here and addres from the top of the compiler stack (don't pop)
	- ]: -> pop address from the assembler stack, write current address there
	"""
	out = []
	stack = []
	pos = 0
	#print(tokens) # XXX
	for t in tokens:
		if not is_linker_token(t):
			out += [t]
			pos += 1
		elif t=='@[':
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
		elif t=='@@':
			#val = pos - int(out[-2]) - 3 # after @]
			target,i = stack[-1] # before @]
			val = pos - target - 2 # before @]
			out += [str(val)]
			pos += 1
		elif t[0]=='@':
			label = t[1:]
			p = pos_by_label[label]
			out += [str(p)]
			pos += 1
		elif t[-1]==":":
			pass
		else:
			raise Exception(f"Unknown linker token `{t}`")
	return out

# -----------------------------------------------------------------------------

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Link MortyVM assembler output into textual p-code')
	parser.add_argument('-o',  metavar='path', type=str, help='output path (default: stdout)')
	parser.add_argument('-f',  metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-d',  action='store_true', help='debug')
	args = parser.parse_args()

	if args.f:
		code = open(args.f,'r').read()
	else:
		code = sys.stdin.read()		
	
	# MAIN
	tokens = tokenize(code)
	tokens = link(tokens)
	output = serialize(tokens)
	
	if args.d:
		print(f"TODO: debug", file=sys.stderr) # TODO: print debug info
	if args.o:
		open(args.o,'w').write(output)
	else:
		print(output)
