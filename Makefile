CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

TARGET = allocate
OBJECTS = allocate.o pqueue.o 

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)


.PHONY: clean purge
clean:
	rm -f *.o
purge:
	rm -f *.o
	rm -f $(TARGET)