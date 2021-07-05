cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -f random.asm -d -O -o random.mrto
python3 ../../link.py -f random.mrto -o random.mrt
vm random.mrt
