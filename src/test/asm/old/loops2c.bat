cd ../../c
gcc vm2c.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm2c.py -hl -i loops.hla -d -o loops2c.mrt
vm loops2c.mrt
