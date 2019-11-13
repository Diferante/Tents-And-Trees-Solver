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
	printf "Starting checks;\n"
	find tests/ -maxdepth 1 -name "*.camp0" -exec valgrind -q --leak-check=full ./$(TARGET) {} \;
	printf "done;\n"

check:
	printf "Starting checks;\n"
	find tests/ -maxdepth 1 -name "*.tents0" -execdir diff {} s/{} \; -execdir echo {} \;
	printf "done;\n"

clean:
	rm -f *.o
