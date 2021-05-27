from basevm import Test1VM
import asm

op_names = "stop _ call ret jz push _ mul add".split(' ')

code1 = asm.to_cells("""
	push 5
	push 15
	add 0
	push 2
	mul 0
	push 1
	push 1
	add 0
	add 0
	stop 0
""", op_names)

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
	