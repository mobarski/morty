from time import perf_counter as time
from collections import defaultdict
import re

class BaseVM:
	'''Base VM:
	   - Operands with 0 arguments are treated the same as operands with 1 argument
	     both always occupy 2 cells (simplification for easier development
	   - both stacks grow up
	   - top op stack register (T)
	'''
	MEM_CELLS = 2024

	def __init__(self, op_names):
		self.i = 0 # instruction pointer
		self.s = 0 # stack pointer
		self.r = 0 # return stack pointer
		self.f = 0 # frame pointer
		self.t = 0 # TOP OF STACK
		self.mem = [0]*self.MEM_CELLS
		self.code = []
		self.time = time()
		self.op_cnt = defaultdict(int)
		self.op_name = {i:n.replace('$','') for i,n in enumerate(op_names)}

	def tick(self):
		"""dispatch based on op to name dictionary"""
		op,arg = self.code[self.i],self.code[self.i+1]
		self.i += 2
		name = self.op_name[op]
		self.op_cnt[name] += 1
		fun = getattr(self, f"op_{name}")
		fun(arg)
		return name,op,arg
	
	# ---[ MICRO OPS ]---------------------------------------------------------

	def push(self, v):
		self.mem[self.s+1] = self.t
		self.t = v
		self.s += 1
	
	def pop(self):
		v = self.t
		self.t = self.mem[self.s]
		self.s -= 1
		return v

	# ---[ CORE OPS WITH ARGUMENT ]--------------------------------------------

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

	# ---[ CORE STACK OPS ]----------------------------------------------------

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

	def op_dup(self, arg):
		self.push(self.t)

	def op_swap(self, arg):
		self.t, self.mem[self.s] = self.mem[self.s], self.t
	
	
class MemOps:

	def op_get(self, arg):
		self.t = self.mem[self.t]

	def op_set(self, arg):
		a = self.pop()
		v = self.pop()
		self.mem[a] = v

	# TODO: bez allot / alloc / variable nie ma sensu ?

class DebugOps:

	def op_stop(self, arg):
		self.i -= 2
	
	def op_clock(self, arg):
		v = int((time()-self.time)*1_000_000)
		self.push(v)
	
	def show_mem(self, lo, hi, end='\n'):
		print(f"MEM:{lo} -> ",end='')
		for x in range(0,hi-lo):
			print(self.mem[lo+x],end=' ')
		print('',end=end)

# ---[ TESTS ]---------------------------------------------------------------------

class Test1VM(BaseVM,MemOps,DebugOps):

	def op_add(self, arg):
		v = self.pop()
		self.t += v

	def op_mul(self, arg):
		v = self.pop()
		self.t *= v
	
	def op_xor(self, arg):
		v = self.pop()
		self.t ^= v

	def op_and(self, arg):
		v = self.pop()
		self.t &= v

	def op_vget(self, arg):
		v = self.mem[self.f + arg]
		self.push(v)

	def op_vset(self, arg):
		v = self.pop()
		self.mem[self.f + arg] = v

	# v 1 add :v
	def op_vincr(self, arg):
		self.mem[self.f + arg] += 1

# -----------------------------------------------------------------------------

