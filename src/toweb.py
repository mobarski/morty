import sys
import argparse
import re
from textwrap import dedent

# -----------------------------------------------------------------------------

def tokenize(code):
	tokens = re.split('\s+', code.strip())
	return tokens

def serialize(tokens):
	text = ','.join([str(t) for t in tokens])
	return text

if __name__=="__main__":
	parser = argparse.ArgumentParser(description='Export MortyVM p-code as html')
	parser.add_argument('-o',  metavar='path', type=str, help='output path (default: stdout)')
	parser.add_argument('-f',  metavar='path', type=str, help='input path (default: stdin)')
	parser.add_argument('-d',  action='store_true', help='debug')
	parser.add_argument('-e',  action='store_true', help='embed dependencies')
	args = parser.parse_args()

	if args.f:
		code = open(args.f,'r').read()
	else:
		code = sys.stdin.read()		
	
	if args.e:
		dependencies = "<script>\n"
		for path in ['js/vm.js','js/io.js','js/screen.js']:
			dependencies += dedent(open(path).read())
		dependencies += "</script>\n"
	else:
		dependencies = dedent("""
			<script src="vm.js"></script>
			<script src="io.js"></script>
			<script src="screen.js"></script>
		""")
	tokens = tokenize(code)
	rom = serialize(tokens)
	html = f"""
		{dependencies}
		<div id="screen"></div>
		<a onclick='fullscreen()'>fullscreen</a/>

		<script>
			rom = [{rom}]
			boot(rom)
			run()
		</script>
	"""
	output = dedent(html)
	
	if args.o:
		open(args.o,'w').write(output)
	else:
		print(output)
