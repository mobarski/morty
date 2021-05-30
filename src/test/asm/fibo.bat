cd ../../c
gcc vm1.c -O3 -o ../test/asm/vm1.exe
cd ../test/asm

python3 ../../asm.py -hl -i fibo.hla -d -o fibo.mrt
vm1 fibo.mrt
