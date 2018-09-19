vm.exe: vm.c vm.o
	gcc -Wall -ansi -pedantic -c vm.c
	gcc -o vm.exe vm.o
