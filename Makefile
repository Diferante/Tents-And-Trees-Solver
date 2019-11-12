# compiler

CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = campista

# sources

SOURCES = CAMPistA.c variants.c


# object

OBJECT = CAMPistA.o



campista: $(OBJECTS)

	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)


CAMPistA.o: CAMPistA.c variants.c variants.h

   


