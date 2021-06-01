cd ../../c
gcc vm2c.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm2c.py -hl -i fibo.hla -d -o fibo2c.mrt
vm fibo2c.mrt
