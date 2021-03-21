allocate: allocate.c queue.o
	gcc -o allocate allocate.c queue.o -g
queue.o: queue.c queue.h
	gcc -c -Wall queue.c -g
