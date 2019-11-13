# compiler

CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = campista
export TARGET

# objects (make automatically makes x.o from x.c)
OBJECTS = CAMPistA.o variants.o


campista: $(OBJECTS)

	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
test:
	find tests/ -maxdepth 1 -name "*.camp0" -exec valgrind -q --leak-check=full ./$(TARGET) {} \;

check:
	find tests/ -maxdepth 1 -name "*.tents0" -exec diff {} s/{} \;

clean:
	rm -f *.o
