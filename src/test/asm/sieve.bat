cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -i sieve.asm -o sieve.mrto
python3 ../../link.py -i sieve.mrto -o sieve.mrt
vm -mem 100000 sieve.mrt
