make: bpop
astack:
	gcc -c -o inc/astack.o inc/astack.c
bboard:
	gcc -std=c99 inc/bboard.c -c -o inc/bboard.o -lncurses
bpop:astack bboard
	gcc -std=c99 bpop.c -g -o bpop inc/astack.o inc/bboard.o -lncurses
