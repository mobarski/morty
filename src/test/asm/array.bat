cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -f array.asm -o array.mrto
python3 ../../link.py -f array.mrto -o array.mrt
vm -mem 1000000 array.mrt
