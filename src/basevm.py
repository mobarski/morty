from time import perf_counter as time
from collections import defaultdict
import re

OP_NAMES = re.findall("\w+","""
	stop
	
	ext push call jz
	fget fset tadd

	ret
	dup drop swap
	stor rtos
	add
	xor and nz
	get set frame

	rot over
	sub mul div mod
	or ltz
	ralloc

	eq ne le ge lt gt
	neg

	clock dot emit
""")


OP_NAME = {i:name for i,name in enumerate(OP_NAMES)}

def asm(text):
	tokens = re.findall("\w+",text.lower())
	out = []
	for t in tokens:
		if t in OP_NAMES:
			out += [OP_NAMES.index(t)]
			continue
		try:
			out += [int(t)]
		except:
			raise Exception(f"unknown token: {t}")
	return out

# -----------------------------------------------------------------------------

class BaseVM:
	'''Base VM:
	   - Operands with 0 arguments are treated the same as operands with 1 argument
	     both always occupy 2 cells (simplification for easier development
	   - both stacks grow up
	'''
	MEM_CELLS = 2024

	def __init__(self):
		self.i = 0 # instruction pointer
		self.s = 0 # stack pointer
		self.r = 0 # return stack pointer
		self.f = 0 # frame pointer
		self.mem = [0]*self.MEM_CELLS
		self.code = []
		self.time = time()
		self.op_cnt = defaultdict(int)

	def tick(self):
		"""dispatch based on op to name dictionary"""
		op,arg = self.code[self.i],self.code[self.i+1]
		self.i += 2
		name = OP_NAME[op]
		self.op_cnt[name] += 1
		fun = getattr(self, f"op_{name}")
		fun(arg)
		return name,op,arg

	# --------------------------------------------------------------------------
	
	def show_mem(self, lo, hi, end='\n'):
		print(f"MEM:{lo} -> ",end='')
		for x in range(0,hi-lo):
			print(self.mem[lo+x],end=' ')
		print('',end=end)
	
	# --------------------------------------------------------------------------
		
	def op_push(self, arg):
		self.push(arg)

	def op_jz(self, arg):
		v = self.pop()
		if not v:
			self.i += arg
	
	def op_call(self, arg):
		self.mem[self.r+1] = self.f
		self.mem[self.r+2] = self.i
		self.r += 2
		self.i = arg
		self.f = self.r
		# TODO: czy moze tablica posrednia w RAM i wektorowanie ???
		# bedzie mozna podmienic i token bedzie malo zajmowal
		# + instrukcja zwracajaca adres tablicy ???
	
	def op_ret(self, arg):
		self.r = self.f - 2
		self.i = self.mem[self.f]
		self.f = self.mem[self.f-1]

	# ---[ REGISTER INDEPENDENT ]----------------------------------------------
	
	def op_stor(self, arg):
		v = self.pop()
		self.mem[self.r+1] = v
		self.r += 1

	def op_rtos(self, arg):
		v = self.mem[self.r]
		self.push(v)
		self.r -= 1

	def op_drop(self, arg):
		self.pop()
	
	# ---[ DEBUG ]-------------------------------------------------------------

	def op_stop(self, arg):
		self.i -= 2
	
	def op_clock(self, arg):
		v = int((time()-self.time)*1_000_000)
		self.push(v)

# -----------------------------------------------------------------------------

class Base1VM(BaseVM):	
	'''Base VM with 1 stack elements register:
	     T - top op stack
	'''
	
	def __init__(self):
		self.t = 0 # TOP OF STACK
		super().__init__()
	
	def push(self, v):
		self.mem[self.s+1] = self.t
		self.t = v
		self.s += 1
	
	def pop(self):
		v = self.t
		self.t = self.mem[self.s]
		self.s -= 1
		return v


class Base2VM(BaseVM):	
	'''Base VM with 2 stack elements registers:
	     T - top op stack
	     N - next on stack
	'''
	
	def __init__(self):
		self.t = 0 # TOP OF STACK
		self.n = 0 # NEXT ON STACK
		super().__init__()
	
	def push(self, v):
		self.mem[self.s+1] = self.n
		self.n = self.t
		self.t = v
		self.s += 1
	
	def pop(self):
		v = self.t
		self.t = self.n
		self.n = self.mem[self.s]
		self.s -= 1
		return v


# ---[ TESTS ]---------------------------------------------------------------------

class Test1VM(Base1VM):

	def op_dup(self, arg):
		self.push(self.t)

	def op_add(self, arg):
		v = self.pop()
		self.t += v
	
	def op_sub(self, arg):
		v = self.pop()
		self.t -= v

	def op_xor(self, arg):
		v = self.pop()
		self.t ^= v

	def op_and(self, arg):
		v = self.pop()
		self.t &= v

	def op_get(self, arg):
		self.t = self.mem[self.t]

	def op_set(self, arg):
		a = self.pop()
		v = self.pop()
		self.mem[a] = v

class Test2VM(Base2VM,Test1VM):
		
	def op_swap(self, arg):
		self.t, self.n = self.n, self.t

	def op_over(self, arg):
		self.push(self.n)
		

# -----------------------------------------------------------------------------

