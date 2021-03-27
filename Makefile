CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

TARGET = allocate
OBJECTS = pqueue.o process.o schedulers.o simulator.o main.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)


.PHONY: clean purge
clean:
	rm -f *.o
purge:
	rm -f *.o
	rm -f $(TARGET)