CFLAGS := -std=c99
make: bpop
astack:
	gcc -c -o inc/astack.o inc/astack.c
bboard:
	gcc inc/bboard.c -c $(CFLAGS) -o inc/bboard.o -lncurses
bpop:astack bboard
	gcc bpop.c -g $(CFLAGS) -o bpop inc/astack.o inc/bboard.o -lncurses
install:
	sudo yum install gcc
	sudo yum install ncurses-devel
	make
clean:
	rm -f ./inc/*.o
