import re

def to_hla(text):
	text = strip_comments(text)
	tokens = tokenize(text)
	ops = compile(tokens)
	hla = ' '.join([op['hla'] for op in ops])
	return hla

# ------------------------------------------------------------------------------

def strip_comments(text):
	text = re.sub('(?ms)[(]{2}.*?[)]{2}', ' ', text) # multiline comments (( ... ))
	text = re.sub('\s[(].*?[)]', ' ', text)
	text = re.sub('\s[(].*', ' ', text)
	return text

# TODO: preserve leading whitespace (indent)
def tokenize(text):
	return re.findall('[^ \t\r\n]+|\n',text.strip())

ops = ['add','mul','div','sub','dot','lt'] # TODO: from asm.py
def compile(tokens):
	out = []
	functions = []
	local = []
	i = 0
	while i<len(tokens):
		t = tokens[i]
		i += 1
		if t=='\n':
			out += [dict(hla='\n')]
		elif t=='def':
			name = tokens[i]
			functions += [name]
			out += [dict(hla=f'goto.@[ {name}:')]
			i += 1
		elif t=='end':
			local = []
			out += [dict(hla='ret ]:')]
		elif t=='times':
			out += [dict(hla='times.@[')]
		elif t=='loop':
			out += [dict(hla='loop.@]')]
		elif t=='then':
			out += [dict(hla='jz.@[')]
		elif t=='do':
			out += [dict(hla=']:')]
		elif t[0]==':':
			# TODO: reserve frame cells right after "def"
			name = t[1:]
			if name not in local:
				local += [name]
			fi = 1+local.index(name)
			out += [dict(hla=f'vset.{fi}')]
		elif t in local:
			fi = 1+local.index(t)
			out += [dict(hla=f'vget.{fi}')]
		elif t in functions:
			out += [dict(hla=f':{t}')]
		elif t in ops:
			out += [dict(hla=t)]
		else:
			out += [dict(hla=t)]
	return out

# ------------------------------------------------------------------------------

if __name__=="__main__":
	x = to_hla("""
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
