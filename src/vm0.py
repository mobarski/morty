
class CanonicalVM:
	"""Morty VM prototype
	"""
	RAM_SIZE = 32
	def __init__(self):
		self.i = 0
		self.r = self.RAM_SIZE-1
		self.s = 0
		self.tos = 0 # TODO !!!
		self.rom = []
		self.ram = [0]*self.RAM_SIZE
	
	def tick(self):
		op = self.rom[self.i]
		self.i += 1
		op = op.replace('>','gt')
		self.__getattribute__(f'op_{op}')()
	
	# alu
	
	def op_add(self):
		self.ram[self.s-1] += self.ram[self.s]
		self.s -= 1

	def op_sub(self):
		self.ram[self.s-1] -= self.ram[self.s]
		self.s -= 1

	def op_and(self):
		self.ram[self.s-1] &= self.ram[self.s]
		self.s -= 1

	def op_or(self):
		self.ram[self.s-1] |= self.ram[self.s]
		self.s -= 1

	def op_xor(self):
		self.ram[self.s-1] ^= self.ram[self.s]
		self.s -= 1

	# control

	def op_jz(self):
		v = self.ram[self.s]
		self.s -= 1
		if v:
			self.i += 1
		else:
			self.i += self.rom[self.i-1]
	
	def op_push(self):
		self.ram[self.s+1] = self.rom[self.i]
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
		self.ram[self.s+1] = self.ram[self.s]
		self.s += 1
	
	def op_drop(self):
		self.s -= 1
	
	def op_swap(self):
		v = self.ram[self.s]
		self.ram[self.s] = self.ram[self.s-1]
		self.ram[self.s-1] = v
	
	def op_over(self):
		self.ram[self.s+1] = self.ram[self.s-1]
		self.s += 1
		
	# registers
	
	def op_gtr(self): # >r
		self.ram[self.r-1] = self.ram[self.s]
		self.s -= 1
		self.r -= 1 # grows down

	def op_rgt(self): # r>
		self.ram[self.s+1] = self.ram[self.r]
		self.s += 1
		self.r += 1 # grows down


class ExtendedVM(CanonicalVM):

	def op_stop(self):
		self.i -= 1
	
	def op_rot(self):
		v = self.ram[self.s-2]
		self.ram[self.s-2] = self.ram[self.s-1]
		self.ram[self.s-1] = self.ram[self.s]
		self.ram[self.s] = v
	
	def op_mul(self):
		self.ram[self.s-1] *= self.ram[self.s]
		self.s -= 1
	
	def op_div(self):
		self.ram[self.s-1] //= self.ram[self.s]
		self.s -= 1
	
	def op_mod(self):
		self.ram[self.s-1] %= self.ram[self.s]
		self.s -= 1


if __name__=="__main__":
	vm = ExtendedVM()
	vm.rom = ['push',40,'push',2,'add','jz',0,'push',666,'push',777,'push',111,'rot','swap','swap','stop']
	for i in range(11):
		vm.tick()
		print(vm.ram)
