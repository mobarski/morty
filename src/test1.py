from basevm import *

code1 = asm("push 1  push 2  push 3  dup 0  add 0  add 0 add 0      stop 0")

if __name__=="__main__":
	vm = Test2VM()
	vm.s = 0
	vm.r = vm.f = 10
	vm.code = code1
	# --------------------------
	for i in range(20):
		name,op,arg = vm.tick()
		#print(f"T:{vm.t}")
		print(f"N:{vm.n} T:{vm.t}")
		vm.show_mem(0,10)
		vm.show_mem(10,20)
		print(name,arg)
		print()
		if op==0 and arg==0: break
	