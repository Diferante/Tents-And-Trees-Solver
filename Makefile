# compiler

CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = campista

# objects (make automatically makes x.o from x.c)
OBJECTS = CAMPistA.o variants.o


campista: $(OBJECTS)

	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
