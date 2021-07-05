cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -f loops.asm -d -o loops.mrto
python3 ../../link.py -f loops.mrto -o loops.mrt
vm loops.mrt

