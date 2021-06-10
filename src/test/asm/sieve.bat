cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i sieve.hla -O -o sieve.mrt
vm -mem 100000 sieve.mrt
