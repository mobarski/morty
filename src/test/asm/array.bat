cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i array.hla -o array.mrt
vm -mem 1000000 array.mrt

python3 ../../asm.py -i array.asm -o array.mrt
vm -mem 1000000 array.mrt
