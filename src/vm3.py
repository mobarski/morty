from time import perf_counter as time
import re

OP_NAMES = re.findall("\w+","""
	ext push call jz
	fget fset tadd
""")

EXT_NAMES = re.findall("\w+","""
	stop
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


OP_NAME  = {i:name for i,name in enumerate(OP_NAMES)}
EXT_NAME = {i:name for i,name in enumerate(EXT_NAMES)}


def asm(text):
	tokens = re.findall("\w+",text.lower())
	out = []
	for t in tokens:
		if t in OP_NAMES:
			out += [OP_NAMES.index(t)]
			continue
		if t in EXT_NAMES:
			out += [EXT_NAMES.index(t)]
			continue
		try:
			out += [int(t)]
		except:
			raise Exception(f"unknown token: {t}")
	return out

# -----------------------------------------------------------------------------

class BaseVM:
	'''Morty VM - inspired by Niklaus Wirth p-code machine
	v3 -> split into MinimalVM and CoreVM
	'''
	MEM_CELLS = 2024
	
	def __init__(self):
		self.i = 0 # instruction pointer
		self.s = 0 # stack pointer
		self.r = 0 # return stack pointer
		self.f = 0 # frame pointer
		self.t = 0 # TOP OF STACK
		self.mem = [0]*self.MEM_CELLS
		self.code = []
		self.s0 = 0 # base 
		self.r0 = 0 # base
		self.time = time()
	
	def show_stack(self, end='\n'):
		print(f"S:{self.s} -> ",end='')
		for x in range(2,self.s+1):
			print(self.mem[x],end=' ')
		print(self.t, end=end)
	
	def show_mem(self, lo, hi, end='\n'):
		print(f"MEM:{lo} -> ",end='')
		for x in range(0,hi-lo):
			print(self.mem[lo+x],end=' ')
		print('',end=end)
	
	def tick(self):
		op,arg = self.code[self.i],self.code[self.i+1]
		self.i += 2
		name = OP_NAME[op]
		fun = getattr(self, f"op_{name}")
		fun(arg)
		return op,arg
	
	def _push(self):
		self.mem[self.s+1] = self.t
		self.s += 1
	
	def _pop(self):
		v = self.t
		self.t = self.mem[self.s]
		self.s -= 1
		return v
	
	# --------------------------------------------------------------------------
		
	def op_push(self, arg):
		self._push()
		self.t = arg
	
	def op_ext(self, arg):
		name = EXT_NAME[arg].replace('>','gt')
		fun = getattr(self, f"ext_{name}")
		fun()
	
	def op_call(self, arg):
		self.mem[self.r+1] = self.f
		self.mem[self.r+2] = self.i
		self.r += 2
		self.i = arg
		self.f = self.r
		# TODO: czy moze tablica posrednia w RAM i wektorowanie ???
		# bedzie mozna podmienic i token bedzie malo zajmowal
		# + instrukcja zwracajaca adres tablicy ???
	
	def op_jz(self, arg):
		v = self._pop()
		if not v:
			self.i += arg
			

class MinimalVM(BaseVM):

	def ext_ret(self):
		self.r = self.f-2
		self.i = self.mem[self.f]
		self.f = self.mem[self.f-1]

	# STACK

	def ext_dup(self):
		self._push() 

	def ext_drop(self):
		self.t = self.mem[self.s]
		self.s -= 1

	def ext_swap(self):
		self.t, selt.mem[self.s] = self.mem[self.s], self.t
		
	def ext_stor(self):
		v = self._pop()
		self.mem[self.r+1] = v
		self.r += 1
	
	def ext_rtos(self):
		self._push()
		self.t = self.mem[self.r]
		self.r -= 1
	
	def ext_frame(self):
		self._push()
		self.t = self.f
	
	# ARITHMETIC
	
	def ext_add(self):
		self.t = self.mem[self.s] + self.t
		self.s -= 1

	# LOGIC

	def ext_xor(self):
		self.t = self.mem[self.s] ^ self.t
		self.s -= 1

	def ext_and(self):
		self.t = self.mem[self.s] & self.t
		self.s -= 1

	def ext_nz(self):
		self.t = 1 if self.t else 0

	# MEMORY

	def ext_get(self):
		self.t = self.mem[self.t]
	
	def ext_set(self):
		self.mem[self.t] = self.mem[self.s]
		self.t = self.mem[self.s-1]
		self.s -= 2


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------

class BasicVM(MinimalVM):

	def ext_stop(self):
		self.i -= 2

	def op_tadd(self, arg):
		self.t += arg

	# fget -> n frame add get
	def op_fget(self, arg):
		self._push()
		self.t = self.mem[self.f+arg]
	
	# core? >R
	def op_fset(self, arg):
		v = self._pop()
		self.mem[self.f+arg] = v

	# STACK

	# target: bca
	# >R ab -- swap ba -- R> bac -- swap bca
	def ext_rot(self):
		v = self.t
		self.t = self.mem[self.s-1]
		self.mem[self.s-1] = self.mem[self.s]
		self.mem[self.s] = v
	
	# target: aba
	# ab -- >R a -- dup aa -- R> aab -- swap aba
	def ext_over(self):
		self._push()
		self.t = self.mem[self.s-1]
		self.s += 1

	def ext_ralloc(self):
		n = self.t
		self.t = self.r+1
		self.r += n

	# ARITHM

	def ext_sub(self):
		self.t = self.mem[self.s] - self.t
		self.s -= 1

	def ext_mul(self):
		self.t = self.mem[self.s] * self.t
		self.s -= 1

	def ext_div(self):
		self.t = self.mem[self.s] // self.t
		self.s -= 1

	def ext_mod(self):
		self.t = self.mem[self.s] % self.t
		self.s -= 1

	# LOGIC
	
	# core?
	def ext_or(self):
		self.t = self.mem[self.s] | self.t
		self.s -= 1

	# COMPARISON

	# ltz -> 0x80000000 and nz
	def ext_ltz(self):
		self.t = int(self.t < 0)
		self.s -= 1

	# eq -> xor nz not
	def ext_eq(self):
		self.t = int(self.mem[self.s] == self.t)
		self.s -= 1

	# ne -> xor nz
	def ext_ne(self):
		self.t = int(self.mem[self.s] != self.t)
		self.s -= 1

	# le -> gt not
	def ext_le(self):
		self.t = int(self.mem[self.s] <= self.t)
		self.s -= 1

	# ge -> lt not 
	def ext_ge(self):
		self.t = int(self.mem[self.s] >= self.t)
		self.s -= 1

	# lt -> sub 0x80000000 and nz
	def ext_lt(self):
		self.t = int(self.mem[self.s] < self.t)
		self.s -= 1

	# gt -> swap lt
	def ext_gt(self):
		self.t = int(self.mem[self.s] > self.t)
		self.s -= 1

	# LOGIC
	
	# core? neg -> -1 xor 1 add
	def ext_neg(self):
		self.t = -self.t
	
	# AUX
	def ext_clock(self):
		self._push()
		self.t = int((time()-self.time)*1_000_000)
	
	def ext_dot(self):
		v = self._pop()
		print(v,end=' ') # TODO: as int32
	
	def ext_emit(self):
		v = self._pop()
		print(chr(v),end='')

if __name__=="__main__":
	vm = BasicVM()
	vm.code = asm("PUSH 3  PUSH 7  EXT STOR  EXT STOR  FGET 1  FGET 1  CALL 24  PUSH 2  FSET 3  EXT STOP  PUSH 1  EXT RET  0 0 ")
	vm.s0 = vm.s = 0
	vm.r0 = vm.r = vm.f = 10 
	for i in range(20):
		op,arg = vm.tick()
		vm.show_stack()
		vm.show_mem(10,20)
		print()
		if op==0 and arg==0: break
	vm.ext_clock()
	vm.ext_dot()
	print("usec")
