cd ../../c
gcc vm1c.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm1c.py -hl -i fibo.hla -d -o fibo1c.mrt
vm fibo1c.mrt
