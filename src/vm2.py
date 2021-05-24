
STOP=0 # XXX: not in p-code, TODO: replace with EXT,0
PUSH=1
EXT=2 # TODO: EXT=0, STOP -> EXT,0 -> 0,0
FGET=3
FSET=4
CALL=5
TADD=6 # optional, can be replaced with -> push X, ext add, BUT it will speed up attribute operations
pass # free, JUMP=7 in p-code 
JZ=8

OP_NAME={
	STOP:"stop",
	PUSH:"push",
	EXT:"ext",
	CALL:"call",
	JZ:"jz",
	TADD:"tadd",
	FGET:"fget",
	FSET:"fset",
}

# --- EXTENSION ---
RET=0
# STACK
DUP=1
DROP=2
ROT=3
SWAP=4
OVER=5
GTR=6 # >R
RTG=7 # R>
RALLOC=8
# ARITHMETIC
ADD=9
SUB=10
MUL=11
DIV=12
MOD=13
AND=14
OR=15
XOR=16
# MEM
GET=17
SET=18
# COMPARISON
EQ=19
NE=20
LT=21
GT=22
LE=23
GE=24

EXT_NAME={
	RET:"ret",
	RALLOC:"ralloc",
	# STACK
	GTR:">r",
	RTG:"r>",
	DUP:"dup",
	DROP:"drop",
	ROT:"rot",
	OVER:"over",
	SWAP:"swap",
	# ARITHMETIC
	ADD:"add",
	SUB:"sub",
	MUL:"mul",
	DIV:"div",
	MOD:"mod",
	AND:"and",
	OR:"or",
	XOR:"xor",
	# MEM
	GET:"get",
	SET:"set",
	# COMPARISON
	EQ:"eq",
	NE:"ne",
	LT:"lt",
	GT:"gt",
	LE:"le",
	GE:"ge",
}

# -----------------------------------------------------------------------------

class BaseVM:
	'''Morty VM v2 - inspired by Niklaus Wirth p-code machine'''
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
	
	def op_stop(self, arg):
		self.i -= 2
	
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
	
	def op_jz(self, arg):
		v = self._pop()
		if not v:
			self.i = arg
	
	def op_tadd(self, arg):
		self.t += arg
	
	def op_fget(self, arg):
		self._push()
		self.t = self.mem[self.f+arg]
	
	# alternatywa: >R jako op
	def op_fset(self, arg):
		v = self._pop()
		self.mem[self.f+arg] = v


class ExtendedVM(BaseVM):

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

	def ext_rot(self):
		v = self.t
		self.t = self.mem[self.s-1]
		self.mem[self.s-1] = self.mem[self.s]
		self.mem[self.s] = v

	def ext_swap(self):
		self.t, selt.mem[self.s] = self.mem[self.s], self.t
	
	def ext_over(self):
		self._push()
		self.t = self.mem[self.s-1]
		self.s += 1
		
	def ext_gtr(self):
		v = self._pop()
		self.mem[self.r+1] = v
		self.r += 1
	
	def ext_rtg(self):
		self._push()
		self.t = self.mem[self.r]
		self.r -= 1

	def ext_ralloc(self):
		n = self.t
		self.t = self.r+1
		self.r += n
	
	# ARITHMETIC
	
	def ext_add(self):
		self.t = self.mem[self.s] + self.t
		self.s -= 1
	
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
	
	def ext_and(self):
		self.t = self.mem[self.s] & self.t
		self.s -= 1

	def ext_or(self):
		self.t = self.mem[self.s] | self.t
		self.s -= 1

	def ext_xor(self):
		self.t = self.mem[self.s] ^ self.t
		self.s -= 1
	
	def ext_neg(self):
		self.t = -self.t
	
	# COMPARISON
	
	def ext_eq(self):
		self.t = int(self.mem[self.s] == self.t)
		self.s -= 1

	def ext_ne(self):
		self.t = int(self.mem[self.s] != self.t)
		self.s -= 1

	def ext_le(self):
		self.t = int(self.mem[self.s] <= self.t)
		self.s -= 1

	def ext_ge(self):
		self.t = int(self.mem[self.s] >= self.t)
		self.s -= 1

	def ext_lt(self):
		self.t = int(self.mem[self.s] < self.t)
		self.s -= 1

	def ext_gt(self):
		self.t = int(self.mem[self.s] > self.t)
		self.s -= 1

	# MEMORY

	def ext_get(self):
		self.t = self.mem[self.t]
	
	def ext_set(self):
		self.mem[self.t] = self.mem[self.s]
		self.t = self.mem[self.s-1]
		self.s -= 2


if __name__=="__main__":
	vm = ExtendedVM()
	vm.code = [PUSH,10, PUSH,30, PUSH,2, EXT,DUP, EXT,DUP, EXT,DROP, EXT,ADD, 0,0]
	vm.code = [PUSH,10, EXT,DUP, PUSH,1, EXT,ADD, CALL,2, 0,0]
	#vm.code = [PUSH,3, PUSH,7, FSET,1, RADD,1, FSET,2, RADD,1, FGET,1, FGET,1, CALL,24, PUSH,2, FSET,3, 0,0, PUSH,1, EXT,RET, 0,0]
	vm.code = [PUSH,3, PUSH,7, EXT,GTR, EXT,GTR, FGET,1, FGET,1, CALL,24, PUSH,2, FSET,3, 0,0, PUSH,1, EXT,RET, 0,0]
	vm.s0 = vm.s = 0
	vm.r0 = vm.r = vm.f = 10 
	for i in range(20):
		op,arg = vm.tick()
		vm.show_stack()
		vm.show_mem(10,20)
		print()
		if op==STOP: break
