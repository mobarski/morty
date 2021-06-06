cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i fibo.hla -d -o fibo.mrt
python3 ../../asm.py -hl -i fibo.hla -t -o fibo.mr.txt
vm fibo.mrt
