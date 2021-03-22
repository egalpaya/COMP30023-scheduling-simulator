CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

TARGET = allocate
OBJECTS = pqueue.o simulator.o main.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)


.PHONY: clean purge
clean:
	rm -f *.o
purge:
	rm -f *.o
	rm -f $(TARGET)