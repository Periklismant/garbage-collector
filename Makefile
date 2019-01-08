CC=gcc
CFLAGS=-O3 -Wall
HDEPS+=headers/%.h

OBJS=stack.o
	
vm: stack.o code/vm.c
	gcc stack.o code/vm.c -o vm $(CFLAGS)

stack.o: code/stack.c headers/error.h headers/instructions.h headers/stack.h
	gcc -g -O3 -c code/stack.c
clean:
	rm -f vm

distclean:
	rm -f *.o vm
