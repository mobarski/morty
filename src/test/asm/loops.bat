cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i loops.hla -O -o loops.mrt
vm loops.mrt
