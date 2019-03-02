all:
	gcc -Wall -Wpedantic -std=c11 a1.c -g -o aout

test:
	./aout testdir/
