# compiler

CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = campista
export TARGET

# objects (make automatically makes x.o from x.c)
OBJECTS = CAMPistA.o TentsSolver.o generalStack.o


campista: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

debug: CFLAGS = -Wall -std=c99 -g
debug: campista

valtest: 
	rm -f tests/*.tents
	printf "Starting tests;\n"
	find tests/ -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec valgrind -q --leak-check=full ./$(TARGET) {} \;
	printf "done;\n"
test: 
	rm -f tests/*.tents
	printf "Starting tests;\n"
	find tests/ -maxdepth 1 -name "*.camp"  -exec echo {} \; -exec ./$(TARGET) {} \;
	printf "done;\n"

check:
	printf "Starting checks;\n"
	find tests/ -maxdepth 1 -name "*.tents" -execdir echo Errors in {}: \; -execdir diff {} s/{} \;
	printf "done;\n"

testmem:
	reset
	rm -f massifreport
	valgrind  --main-stacksize=20000000 --stacks=yes --threshold=10.0 --massif-out-file=massifreport --tool=massif ./$(TARGET) memtests.camp0 
	ms_print --threshold=10.0 massifreport

testa:
	printf "Starting time check;\n"
	time for n in {1..10}; do ./$(TARGET) teste_017.camp0; done
	diff teste_017.tents0 teste_017.sol
	printf "done;\n"

testtime:
	printf "Starting time check;\n"
	time for n in {1..100}; do ./$(TARGET) memtests.camp0; done
	printf "done;\n"


clean:
	rm -f *.o
	rm -f tests/*.tents
