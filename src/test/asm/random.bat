cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -hl -i random.hla -d -O -o random.mrt
vm random.mrt

python3 ../../asm.py -i random.asm -d -O -o random.mrt
vm random.mrt
