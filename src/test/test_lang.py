import sys; sys.path.append('../')
from lang import *

def test_code(code):
	asm = to_asm(code)
	print(asm)

if 1:
	test_code("""
	((
		this
		is a
		test
	))
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
	( -------------------
		20 foo 2 add
		$x
		x dot
	""")
