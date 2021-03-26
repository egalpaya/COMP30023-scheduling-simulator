CC = gcc
CFLAGS = -Wall -g -lm

TARGET = allocate
OBJECTS = pqueue.o process.o shortest_time.o simulator.o main.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)


.PHONY: clean purge
clean:
	rm -f *.o
purge:
	rm -f *.o
	rm -f $(TARGET)