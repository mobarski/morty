cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -i fibo.asm -d -O -o fibo.mrto
python3 ../../link.py -i fibo.mrto -o fibo.mrt
vm fibo.mrt
