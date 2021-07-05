cd ../../c
gcc vm.c -O3 -o ../test/morty/vm.exe
cd ../test/morty

python3 ../../lang.py -f %1.morty | python3 ../../asm.py | python3 ../../link.py -o %1.mrt
vm -mem 100000 %1.mrt
