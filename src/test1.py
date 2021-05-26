from basevm import Test1VM
from vmasm import asm_to_cells

op_names = "stop _ call ret jz push _ mul add".split(' ')

code1 = asm_to_cells("5 15 add 2 mul 1 1 add add stop", op_names)

if __name__=="__main__":
	vm = Test1VM(op_names)
	vm.s = 0
	vm.r = vm.f = 10
	vm.code = code1 # <---[ SELECT CODE HERE ]
	# --------------------------
	for i in range(20):
		name,op,arg = vm.tick()
		print(f"T:{vm.t}")
		vm.show_mem(0,10)
		vm.show_mem(10,20)
		print(name,arg)
		print()
		if op==0 and arg==0: break
	