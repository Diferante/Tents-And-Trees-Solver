# compiler

CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = campista
export TARGET

# objects (make automatically makes x.o from x.c)
OBJECTS = CAMPistA.o variants.o charStack.o


campista: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

test:
	rm -f tests/*.tents0
	printf "Starting tests;\n"
	find tests/ -maxdepth 1 -name "*.camp0"  -exec echo {} \; -exec valgrind -q --leak-check=full ./$(TARGET) {} \;
	printf "done;\n"

check:
	printf "Starting checks;\n"
	find tests/ -maxdepth 1 -name "*.tents0" -execdir echo Errors in {}: \; -execdir diff {} s/{} \; 
	printf "done;\n"

testmem: 
	reset
	rm -f massifreport
	valgrind  --main-stacksize=20000000 --stacks=yes --threshold=10.0 --massif-out-file=massifreport --tool=massif ./$(TARGET) memtests.camp0 
	ms_print --threshold=10.0 massifreport 

testtime:
	printf "Starting time check;\n"
	time for n in {1..100}; do ./$(TARGET) memtests.camp0; done
	printf "done;\n"


clean:
	rm -f *.o
