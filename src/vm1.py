class BaseVM:
	"""Morty VM prototype with TOS register
	   simplification: ignores cell limits / structure
	"""
	RAM_SIZE = 100
	def __init__(self):
		self.i = 0
		self.r = 32
		self.s = 0
		self.tos = 0 # top-of-stack register
		self.rom = []
		self.ram = [0]*self.RAM_SIZE
	
	def tick(self):
		op = self.rom[self.i]
		self.i += 1
		op = op.replace('>','gt')
		self.op = op # XXX
		self.__getattribute__(f'op_{op}')()
	
	def show_s(self):
		print(f"S:{self.s} ->", end=' ')
		for s in range(2,self.s+1):
			print(self.ram[s], end=' ')
		if self.s:
			print(self.tos, end=' ')
		print()

	def show(self):
		print(f"I:{self.i} R:{self.r} B:{self.b} OP:{self.op}")
			
class CanonicalVM(BaseVM):

	# alu
	
	def op_add(self):
		self.tos += self.ram[self.s]
		self.s -= 1

	def op_sub(self):
		self.tos = self.ram[self.s] - self.tos
		self.s -= 1

	def op_and(self):
		self.tos &= self.ram[self.s]
		self.s -= 1

	def op_or(self):
		self.tos |= self.ram[self.s]
		self.s -= 1

	def op_xor(self):
		self.tos ^= self.ram[self.s]
		self.s -= 1

	# control

	def op_jz(self):
		v = self.tos
		self.tos = self.ram[self.s]
		self.s -= 1
		if v:
			self.i += 1
		else:
			self.i += self.rom[self.i-1]
	
	def op_push(self):
		self.ram[self.s+1] = self.tos
		self.tos = self.rom[self.i]
		self.s += 1
		self.i += 1
	
	def op_call(self):
		self.ram[self.r] = self.i+1
		self.r -= 1 # grows down
		self.i = self.rom[self.i]
	
	def op_ret(self):
		self.i = self.ram[self.r]
		self.r += 1 # grows down
	
	# stack
	
	def op_dup(self):
		self.ram[self.s+1] = self.tos
		self.s += 1
	
	def op_drop(self):
		self.tos = self.ram[self.s]
		self.s -= 1
	
	def op_swap(self):
		v = self.tos
		self.tos = self.ram[self.s]
		self.ram[self.s] = v
	
	def op_over(self):
		self.ram[self.s+1] = self.tos
		self.tos = self.ram[self.s]
		self.s += 1
		
	# registers
	
	def op_gtr(self): # >r
		self.ram[self.r-1] = self.tos
		self.tos = self.ram[self.s]
		self.s -= 1
		self.r -= 1 # grows down

	def op_rgt(self): # r>
		self.ram[self.s+1] = self.tos
		self.tos = self.ram[self.r]
		self.s += 1
		self.r += 1 # grows down
	
	# memory
	
	def op_get(self):
		self.tos = self.ram[self.tos]

	def op_set(self):
		self.ram[self.tos] = self.ram[self.s]
		self.tos = self.ram[self.s-2]
		self.s -= 2

class ExtendedVM(CanonicalVM):

	def op_stop(self):
		self.i -= 1
	
	def op_nop(self):
		pass
	
	# (abc--bca)
	def op_rot(self):
		v = self.ram[self.s-1]
		self.ram[self.s-1] = self.ram[self.s]
		self.ram[self.s] = self.tos
		self.tos = v
	
	def op_mul(self):
		self.tos *= self.ram[self.s]
		self.s -= 1
	
	def op_div(self):
		self.tos = self.ram[self.s] // self.tos
		self.s -= 1
	
	def op_mod(self):
		self.tos = self.ram[self.s] % self.tos
		self.s -= 1

class LocalsVM(ExtendedVM):
	def __init__(self):
		self.b = 0
		super().__init__()

	def op_local(self):
		v = self.rom[self.i]
		self.i += 1
		self.ram[self.s+1] = self.tos
		self.s += 1
		self.tos = self.ram[self.b-v] # grows down

	# TODO: as xcall, used only when needed
	def op_call(self):
		self.ram[self.r-1] = self.b
		self.ram[self.r-2] = self.i+1
		self.r -= 2 # grows down
		self.b = self.r
		self.i = self.rom[self.i]
	
	# TODO: as xret, used only when needed
	def op_ret(self):
		self.i = self.ram[self.b]
		self.b = self.ram[self.b+1] # grows down
		self.r += 2 # grows down

if __name__=="__main__":
	vm = LocalsVM()
	vm.rom = ['push',40,'push',2,'add','jz',0,'push',666,'push',777,'push',111,'rot','swap','swap','dot','dot','stop']
	vm.rom = ['push',42,'push',0,'poke','push',111,'stop']
	vm.rom = ['nop','push',20,'push',2,'mul','push',2,'add','push',11,'sub','push',3,'div','push',6,'mod','push',1,'push',2,'rot','swap','over','add','add','add','stop']
	vm.rom = ['push',2,'call',8,'push',775,'add','stop','push',111,'nop','gtr','local',1,'ret']
	for i in range(10):
		vm.tick()
		vm.show_s()
		vm.show()
		print()
		
