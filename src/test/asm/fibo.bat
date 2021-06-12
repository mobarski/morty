cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i fibo.hla -d -O -o fibo.mrt
vm fibo.mrt
