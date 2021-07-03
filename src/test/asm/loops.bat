cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -i loops.asm -d -o loops.mrto
python3 ../../link.py -i loops.mrto -o loops.mrt
vm loops.mrt

