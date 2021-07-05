cd ../../c
gcc vm.c -O3 -o ../test/asm/vm.exe
cd ../test/asm

python3 ../../asm.py -f %1.asm -d | python3 ../../link.py -o %1.mrt
vm -mem 100000 %1.mrt
